//

#include "new_pe_builder.h"
#include "import_packer.h"
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
    auto importBlock = std::find_if(
      newSections_.additionalDataBlocks.cbegin()
      , newSections_.additionalDataBlocks.cend()
      ,[](const auto& block)->auto
    {
      return block.ownerType == PackerType::kImportPacker;
    });

    if (importBlock == newSections_.additionalDataBlocks.cend())
    {
      throw std::runtime_error("Cant find new import block");
    }
    nPeh.setIddImportRva(importBlock->virtualOffset);
    nPeh.setIddImportSize(importBlock->size);

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
template <class OutFileBufferType, class OffsetType, class NewImportsContainerType>
void ReplaceContainerData(OutFileBufferType& outFileBuffer, OffsetType rawOffset, NewImportsContainerType& new_imports)
{
  // removing preallocated buffer
  outFileBuffer.erase(
    outFileBuffer.begin() + rawOffset
    , outFileBuffer.begin() + rawOffset + new_imports.size());
  // inserting new import data
  outFileBuffer.insert(
    outFileBuffer.begin() + rawOffset
    , new_imports.cbegin(), new_imports.cend());
}

//==================================================================================

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

  decltype(newSections_.additionalDataBlocks) importsBlocks;
  auto importBlocks = std::copy_if(
    newSections_.additionalDataBlocks.cbegin()
    , newSections_.additionalDataBlocks.cend()
    , importsBlocks.begin()
    , [](const auto& block)->auto
  {
    return block.ownerType == PackerType::kImportPacker;
  });

  for (auto& importBlock: importsBlocks)
  {
    if (importBlock.packerParam == (int32_t)ImportBlockTypes::kNewImportData)
    {
      ReplaceContainerData(outFileBuffer, importBlock.rawOffset, newImports_.new_imports);
    }
    else
    if (importBlock.packerParam == (int32_t)ImportBlockTypes::kNewImportData)
    {
      ReplaceContainerData(outFileBuffer, importBlock.rawOffset, newImports_.old_imports);
    }
    else
    {
      return Expected<std::vector<uint8_t>>::fromException(std::runtime_error("eek unexpected buffer type"));
    }
  }

  //--------------------------------------------------------------------------------

  return outFileBuffer;
}
