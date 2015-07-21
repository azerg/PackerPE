//
#include "sections_packer.h"

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
    // test only
    sectionInfo.newHeader = sectionInfo.originalHeader;
    sectionInfo.data.assign(sectionDataBegin, sectionDataBegin + cbSection);

    uint32_t sizeRequest = 0;
    for (const auto& blockRequest: additionalSizeRequest)
    {
      sizeRequest += blockRequest.size;
    }
    //------------------------------------------------------------------------------

    // saving section data
    sectionsOut.push_back(std::move(sectionInfo));

    cbSections += cbSection;
  }

  //------------------------------------------------------------------------------
  // test only
  uint32_t sizeRequest = 0;
  for (const auto& blockRequest : additionalSizeRequest)
  {
    sizeRequest += blockRequest.size;
  }
  //------------------------------------------------------------------------------
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

Expected<ErrorCode> SectionsPacker::IsReady() const
{
  return ErrorCode::ERROR_SUCC;
}