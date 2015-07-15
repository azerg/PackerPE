#pragma once

#include "sections_packer.h"

#include "file_utils.h"

template<int bits>
void rebuildSections(
  PeLib::PeFile& peFileSrc
  , const std::vector<uint8_t>& sourceFileBuff
  , SectionsArr& sectionsOut
  , uint32_t& offsetInOut)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFileSrc).peHeader();
  uint32_t cbSections{};

  auto nSections = peh.calcNumberOfSections();
  for (auto idxSection = 0; idxSection < nSections; idxSection++)
  {
    //peh.readSections

    auto cbSection = peh.getSizeOfRawData(idxSection);

    /*
    file_utils::appendToFile(
      outFileName.c_str()
      , reinterpret_cast<const char*>(&sourceFileBuff.at(peh.getPointerToRawData(idxSection)))
      , cbSection);
    */

    cbSections += cbSection;
  }
}

class DumpSectionsVisitor: public PeLib::PeFileVisitor
{
public:
  DumpSectionsVisitor(const std::vector<uint8_t>& sourceFileBuff, SectionsArr& sectionsOut, uint32_t dataOffset):
    sourceFileBuff_(sourceFileBuff)
    , sectionsOut_(sectionsOut)
    , dataOffset_(dataOffset)
  {}
  virtual void callback(PeLib::PeFile32& file)
  {
    rebuildSections<32>(file, sourceFileBuff_, sectionsOut_, dataOffset_);
  }
  virtual void callback(PeLib::PeFile64& file)
  {
    rebuildSections<64>(file, sourceFileBuff_, sectionsOut_, dataOffset_);
  }
private:
  const std::vector<uint8_t>& sourceFileBuff_;
  SectionsArr& sectionsOut_;
  uint32_t dataOffset_;
};

SectionsArr SectionsPacker::ProcessExecutable(uint32_t dataOffset)
{
  auto sourceFileBuff = file_utils::readFile(srcPEFile_->getFileName());

  SectionsArr newSections;

  DumpSectionsVisitor sectionsVisitor(sourceFileBuff, newSections, dataOffset);
  srcPEFile_->visit(sectionsVisitor);

  return newSections;
}

Expected<ErrorCode> SectionsPacker::IsReady() const
{
  return ErrorCode::ERROR_SUCC;
}