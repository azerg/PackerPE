#pragma once

#include "sections_packer.h"

#include "file_utils.h"

template<int bits>
void rebuildSections(
  PeLib::PeFile& peFileSrc
  , const std::vector<uint8_t>& sourceFileBuff
  , SectionsArr& sectionsOut)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFileSrc).peHeader();

  uint32_t cbSections{};
  auto nSections = peh.calcNumberOfSections();
  for (auto idxSection = 0; idxSection < nSections; idxSection++)
  {
    Section sectionInfo;
    sectionInfo.original_header = peh.getSectionHeader(idxSection);
    auto cbSection = peh.getSizeOfRawData(idxSection);

    auto sectionDataBegin = &sourceFileBuff.at(peh.getPointerToRawData(idxSection));

    // test only
    sectionInfo.new_header = sectionInfo.original_header;
    sectionInfo.data.assign(sectionDataBegin, sectionDataBegin + cbSection);

    // saving section data
    sectionsOut.push_back(std::move(sectionInfo));

    cbSections += cbSection;
  }
}

class DumpSectionsVisitor: public PeLib::PeFileVisitor
{
public:
  DumpSectionsVisitor(const std::vector<uint8_t>& sourceFileBuff, SectionsArr& sectionsOut):
    sourceFileBuff_(sourceFileBuff)
    , sectionsOut_(sectionsOut)
  {}
  virtual void callback(PeLib::PeFile32& file)
  {
    rebuildSections<32>(file, sourceFileBuff_, sectionsOut_);
  }
  virtual void callback(PeLib::PeFile64& file)
  {
    rebuildSections<64>(file, sourceFileBuff_, sectionsOut_);
  }
private:
  const std::vector<uint8_t>& sourceFileBuff_;
  SectionsArr& sectionsOut_;
};

SectionsArr SectionsPacker::ProcessExecutable()
{
  auto sourceFileBuff = file_utils::readFile(srcPEFile_->getFileName());

  SectionsArr newSections;

  DumpSectionsVisitor sectionsVisitor(sourceFileBuff, newSections);
  srcPEFile_->visit(sectionsVisitor);

  return newSections;
}

Expected<ErrorCode> SectionsPacker::IsReady() const
{
  return ErrorCode::ERROR_SUCC;
}