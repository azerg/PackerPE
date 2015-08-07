//

#include "new_pe_builder.h"
#include <iostream>

uint32_t rebuildMZHeader(PeFilePtr& peFile, std::vector<uint8_t>& outFileBuffer, std::vector<uint8_t>& sourceFileBuff)
{
  std::vector<PeLib::byte> mzHeadBuffer;
  auto mzHead = peFile->mzHeader();
  mzHead.rebuild(mzHeadBuffer);

  std::copy(mzHeadBuffer.cbegin(), mzHeadBuffer.cend(), std::back_inserter(outFileBuffer));

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

//------------------------------------------------------------------------

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
  template<int bits>
  void rebuildPEHeader(PeLib::PeFile& peFile)
  {
    const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

    PeLib::PeHeaderT<bits> nPeh(peh); // rebuilding new Pe-header
    //----------------------------------------------

    nPeh.killSections(); // clean section headers
    nPeh.setNumberOfSections(newSections_.sections.size());

    for (const auto& section : newSections_.sections)
    {
      nPeh.addSection(section.newHeader);
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

    //----------------------------------------------

    std::vector<PeLib::byte> peHeaderBuff;
    nPeh.rebuild(peHeaderBuff);

    auto cbAfterHeader = nPeh.size();
    std::copy(peHeaderBuff.cbegin(), peHeaderBuff.cend(), std::back_inserter(outFileBuffer_));

    offset_ += peHeaderBuff.size();

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

// ---------------------------------------------------------------

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
    std::copy(section.data.cbegin(), section.data.cend(), std::back_inserter(outFileBuffer));
  }

  // insert imports data
  auto importBlock = std::find_if(
    newSections_.additionalDataBlocks.cbegin()
    , newSections_.additionalDataBlocks.cend()
    , [](const auto& block)->auto
  {
    return block.ownerType == PackerType::kImportPacker;
  });

  // removing preallocated buffer
  outFileBuffer.erase(
    outFileBuffer.begin() + importBlock->rawOffset
    , outFileBuffer.begin() + importBlock->rawOffset + newImports_.new_imports.size());
  outFileBuffer.insert(
    outFileBuffer.begin() + importBlock->rawOffset
    , newImports_.new_imports.cbegin(), newImports_.new_imports.cend());

  //--------------------------------------------------------------------------------

  return outFileBuffer;
  //return Expected<std::vector<uint8_t>>::fromException(std::runtime_error("eek"));
}
