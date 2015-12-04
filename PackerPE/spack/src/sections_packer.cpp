//
#include <PeLibAux.h>
#include "sections_packer.h"

template <int bits>
void AppendRequiredSizeSection(
  const PeLib::PeHeaderT<bits>& newPeHeader
  , SectionsArr& sectionsOut
  , const std::vector<RequiredDataBlock>& additionalSizeRequest)
{
  // processing all additional data blocks as one (for alpha)
  auto sizeRequest = std::accumulate(additionalSizeRequest.cbegin(), additionalSizeRequest.cend(), 0,
    [](int& left, const RequiredDataBlock& right)
  {
    return left += right.size;
  });

  Section newSectionInfo;
  auto& newSectionHead = newSectionInfo.newHeader;
  //----------------------------------
  // Filling new section header
  
  const char newSectionName[] = ".new\x00\x00\x00";
  memcpy_s(reinterpret_cast<char*>(newSectionHead.Name), sizeof(newSectionHead.Name), newSectionName, sizeof(newSectionName));
  newSectionHead.Characteristics =
    PeLib::PELIB_IMAGE_SCN_MEM_WRITE
    | PeLib::PELIB_IMAGE_SCN_MEM_READ
    | PeLib::PELIB_IMAGE_SCN_MEM_EXECUTE
    | PeLib::PELIB_IMAGE_SCN_CNT_INITIALIZED_DATA
    | PeLib::PELIB_IMAGE_SCN_CNT_CODE;

  auto lastSection = sectionsOut.sections.back();
  newSectionHead.PointerToRawData = lastSection.newHeader.PointerToRawData + lastSection.newHeader.SizeOfRawData;
  newSectionHead.SizeOfRawData = PeLib::alignOffset(sizeRequest, newPeHeader.getFileAlignment());

  newSectionHead.VirtualAddress = lastSection.newHeader.VirtualAddress +
    PeLib::alignOffset(lastSection.newHeader.VirtualSize, newPeHeader.getSectionAlignment());
  newSectionHead.VirtualSize = PeLib::alignOffset(sizeRequest, newPeHeader.getSectionAlignment());

  //----------------------------------
  // Filling new section data (just leaving cave here for further requesters)

  newSectionInfo.data = decltype(newSectionInfo.data)(newSectionHead.SizeOfRawData);

  //----------------------------------
  // Adding new section to list of headers

  sectionsOut.sections.push_back(newSectionInfo);

  //----------------------------------
  // Filling a list of pointers to requested blocks

  auto currentBlocksOffset = 0;
  for (auto& sizeRequestBlock : additionalSizeRequest)
  {
    sectionsOut.additionalDataBlocks.push_back(
    {newSectionHead.PointerToRawData + currentBlocksOffset
      , newSectionHead.VirtualAddress + currentBlocksOffset
      , sizeRequestBlock.size
      , sizeRequestBlock.ownerType
      , sizeRequestBlock.packerParam});

    currentBlocksOffset += sizeRequestBlock.size;
  }
}

template<int bits>
void rebuildSections(
  PeLib::PeFile& peFileSrc
  , const std::vector<uint8_t>& sourceFileBuff
  , const std::vector<RequiredDataBlock>& additionalSizeRequest
  , SectionsArr& sectionsOut)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFileSrc).peHeader();

  uint32_t cbSections{};
  auto nSections = peh.calcNumberOfSections();
  for (auto idxSection = 0; idxSection < nSections; idxSection++)
  {
    Section sectionInfo;
    sectionInfo.originalHeader = peh.getSectionHeader(idxSection);
    auto cbSection = peh.getSizeOfRawData(idxSection);

    auto sectionDataBegin = &sourceFileBuff.at(peh.getPointerToRawData(idxSection));

    //------------------------------------------------------------------------------
    // saving original sections data for testing only (!)
    sectionInfo.newHeader = sectionInfo.originalHeader;
    sectionInfo.data.assign(sectionDataBegin, sectionDataBegin + cbSection);
    //------------------------------------------------------------------------------
    // marking .idata section as writable for testing only (!!!!)

    if (std::string((char*)sectionInfo.originalHeader.Name).compare(".idata") == 0)
    {
      sectionInfo.newHeader.Characteristics = sectionInfo.newHeader.Characteristics | PeLib::PELIB_IMAGE_SCN_MEM_WRITE;
    }

    //------------------------------------------------------------------------------

    // saving section data
    sectionsOut.sections.push_back(std::move(sectionInfo));

    cbSections += cbSection;
  }

  // test only - append required buffer as a new section
  AppendRequiredSizeSection<bits>(peh, sectionsOut, additionalSizeRequest);
}

class DumpSectionsVisitor: public PeLib::PeFileVisitor
{
public:
  DumpSectionsVisitor(const std::vector<uint8_t>& sourceFileBuff, const std::vector<RequiredDataBlock>& additionalSizeRequest, SectionsArr& sectionsOut) :
    sourceFileBuff_(sourceFileBuff)
    , additionalSizeRequest_(additionalSizeRequest)
    , sectionsOut_(sectionsOut)
  {}
  virtual void callback(PeLib::PeFile32& file)
  {
    rebuildSections<32>(file, sourceFileBuff_, additionalSizeRequest_, sectionsOut_);
  }
  virtual void callback(PeLib::PeFile64& file)
  {
    rebuildSections<64>(file, sourceFileBuff_, additionalSizeRequest_, sectionsOut_);
  }
private:
  const std::vector<uint8_t>& sourceFileBuff_;
  const std::vector<RequiredDataBlock>& additionalSizeRequest_;
  SectionsArr& sectionsOut_;
};

SectionsArr SectionsPacker::ProcessExecutable(const std::vector<uint8_t>& sourceFileBuff, const std::vector<RequiredDataBlock> additionalSizeRequest)
{
  SectionsArr newSections;

  DumpSectionsVisitor sectionsVisitor(sourceFileBuff, additionalSizeRequest, newSections);
  srcPEFile_->visit(sectionsVisitor);

  return newSections;
}
