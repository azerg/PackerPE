//

#include "new_pe_builder.h"
#include "import_packer.h"
#include "common_utils.h"
#include <iostream>


uint32_t rebuildMZHeader(PeFilePtr& peFile, std::vector<uint8_t>& outFileBuffer, const std::vector<uint8_t>& sourceFileBuff)
{
  std::vector<PeLib::byte> mzHeadBuffer;
  auto mzHead = peFile->mzHeader();
  mzHead.rebuild(mzHeadBuffer);

  outFileBuffer = std::move(mzHeadBuffer);

  // append mzHeader with undocumented data located between MZHeader & PEHeader
  auto peOffset = mzHead.getAddressOfPeHeader();
  auto cbMZHead = mzHead.size();
  auto cbJunkData = std::max(peOffset - cbMZHead, static_cast<uint32_t>(0));
  if (cbJunkData)
  {
    std::copy(&sourceFileBuff.at(cbMZHead), &sourceFileBuff.at(cbMZHead + cbJunkData), std::back_inserter(outFileBuffer));
  }

  return cbMZHead + cbJunkData;
}

//==============================================================================
template <PackerType ownerTypeId, class newSetionsType>
auto GetAdditionalDataBlocks(const newSetionsType& newSections)
{
  auto targetBlocks = std::find_if(
    newSections.additionalDataBlocks.cbegin()
    , newSections.additionalDataBlocks.cend()
    , [](const auto& block)->auto
  {
    return block.ownerType == ownerTypeId;
  });

  if (targetBlocks == newSections.additionalDataBlocks.cend())
  {
    throw std::runtime_error("Cant find new required data block");
  }

  return targetBlocks;
}

class RebuildPeHeaderVisitor : public PeLib::PeFileVisitor
{
public:
  RebuildPeHeaderVisitor(
    std::vector<uint8_t>& outFileBuffer
    , const SectionsArr& newSections
    , const ImportsArr& newImports
    , uint32_t& offset) :
    outFileBuffer_(outFileBuffer)
    , newSections_(newSections)
    , newImports_(newImports)
    , offset_(offset)
  {}
  virtual void callback(PeLib::PeFile32& file) { rebuildPEHeader<32>(file); }
  virtual void callback(PeLib::PeFile64& file) { rebuildPEHeader<64>(file); }
private:
  template <int bits>
  void UpdateSizeOfImage(PeLib::PeHeaderT<bits> newPeHeader)
  {
    auto lastSectionHead = newSections_.sections.back().newHeader;
    newPeHeader.setSizeOfImage(lastSectionHead.VirtualAddress + lastSectionHead.VirtualSize);
  }

  template<int bits>
  void rebuildPEHeader(PeLib::PeFile& peFile)
  {
    const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

    PeLib::PeHeaderT<bits> nPeh(peh); // rebuilding new Pe-header

    //----------------------------------------------

    nPeh.killSections(); // clean section headers
    nPeh.setNumberOfSections(newSections_.sections.size());

    PeLib::dword sectionsSize = 0;
    for (const auto& section : newSections_.sections)
    {
      nPeh.addSection(section.newHeader);
      sectionsSize += section.newHeader.SizeOfRawData;
    }

    // update PE-file with new import data
    auto importBlock = GetAdditionalDataBlocks<PackerType::kImportPacker>(newSections_);
    nPeh.setIddImportRva(importBlock->virtualOffset);
    nPeh.setIddImportSize(importBlock->size);

    // update PE-file with new IAT address, pointer to our Stub structure, allocated in new file.
    auto stubBlock = GetAdditionalDataBlocks<PackerType::kStubPacker>(newSections_);
    //nPeh.setIddIatRva(stubBlock->virtualOffset);
    //nPeh.setIddIatSize(16); // todo(aserg) REPLACE THIS WITH VALID IAT SIZE, DETECTED IN COMPILE-TIME !!!!!!

    //--------------------------------------------------------------------------------
    // Updating SizeOfImage
    
    UpdateSizeOfImage(nPeh);

    //----------------------------------------------
    // saving new PE-HEad in output buffer

    std::vector<PeLib::byte> peHeaderBuff;
    nPeh.rebuild(peHeaderBuff);

    auto cbAfterHeader = nPeh.size();
    offset_ += peHeaderBuff.size();

    std::copy(std::begin(peHeaderBuff), std::end(peHeaderBuff), std::back_inserter(outFileBuffer_));

    // append with aligned zero-bytes
    auto headersSize = nPeh.getSizeOfHeaders() - offset_;
    if (headersSize)
    {
      //todo(azerg): strip unused data by fixing size of headers ?
      outFileBuffer_.insert(outFileBuffer_.end(), headersSize, 0);
    }
    offset_ += headersSize;
  }

  std::vector<uint8_t>& outFileBuffer_;
  const SectionsArr& newSections_;
  const ImportsArr& newImports_;
  uint32_t& offset_;
};

//==================================================================================

NewPEBuilder::NewPEBuilder(
  PeFilePtr& srcPeFile
  , const std::vector<uint8_t>& sourceFileBuff
  , const std::vector<RequiredDataBlock>& additionalSizeRequest
  , IImportPacker* pImportPacker
  , IStubPacker* pStubPacker
  , ISectionsPacker* pSectionsPacker):
  srcPeFile_(srcPeFile)
  , sourceFileBuff_(sourceFileBuff)
  , additionalSizeRequest_(additionalSizeRequest)
  , importPacker_(pImportPacker)
  , stubPacker_(pStubPacker)
  , sectionsPacker_(pSectionsPacker)
{
  newSections_ = pSectionsPacker->ProcessExecutable(sourceFileBuff, additionalSizeRequest_);
  // new import RVA passed here
  newImports_ = importPacker_->ProcessExecutable(newSections_.additionalDataBlocks);
}

Expected<std::vector<uint8_t>> NewPEBuilder::GenerateOutputPEFile()
{
  std::vector<uint8_t> outFileBuffer;
  auto offset = rebuildMZHeader(srcPeFile_, outFileBuffer, sourceFileBuff_);

  RebuildPeHeaderVisitor peVisitor(outFileBuffer, newSections_, newImports_, offset);
  srcPeFile_->visit(peVisitor);

  //--------------------------------------------------------------------------------
  // configure sections data
  for (const auto& section : newSections_.sections)
  {
    std::move(section.data.cbegin(), section.data.cend(), std::back_inserter(outFileBuffer));
  }

  //--------------------------------------------------------------------------------
  // insert imports data

  AdditionalDataBlocksType importsBlocks;
  std::copy_if(
    newSections_.additionalDataBlocks.cbegin()
    , newSections_.additionalDataBlocks.cend()
    , std::back_inserter(importsBlocks)
    , [](const auto& block)->auto
  {
    return block.ownerType == PackerType::kImportPacker;
  });

  for (auto& importBlock: importsBlocks)
  {
    if (importBlock.packerParam == (int32_t)ImportBlockTypes::kNewImportData)
    {
      utils::ReplaceContainerData(outFileBuffer, importBlock.rawOffset, newImports_.new_imports);
    }
    else
    if (importBlock.packerParam == (int32_t)ImportBlockTypes::kOldImportData)
    {
      utils::ReplaceContainerData(outFileBuffer, importBlock.rawOffset, newImports_.old_imports);
    }
    else
    {
      return Expected<std::vector<uint8_t>>::fromException(std::runtime_error("eek unexpected buffer type"));
    }
  }

  //--------------------------------------------------------------------------------
  // Insert stub data
  stubPacker_->ProcessExecutable(outFileBuffer, newSections_.additionalDataBlocks);

  return outFileBuffer;
}
