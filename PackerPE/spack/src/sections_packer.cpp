#pragma once

#include "sections_packer.h"

#include "file_utils.h"

template<int bits>
void rebuildSections(
  PeLib::PeFile& peFileSrc
  , const std::vector<uint8_t>& sourceFileBuff
  , const std::string& outFileName)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFileSrc).peHeader();
  uint32_t cbSections{};

  auto nSections = peh.calcNumberOfSections();
  for (auto idxSection = 0; idxSection < nSections; idxSection++)
  {
    peh.getSectionName(idxSection);

    auto cbSection = peh.getSizeOfRawData(idxSection);

    file_utils::appendToFile(
      outFileName.c_str()
      , reinterpret_cast<const char*>(&sourceFileBuff.at(peh.getPointerToRawData(idxSection)))
      , cbSection);

    cbSections += cbSection;
  }
}

class DumpSectionsVisitor: public PeLib::PeFileVisitor
{
public:
  DumpSectionsVisitor(const std::vector<uint8_t>& sourceFileBuff, const std::string& outFileName):
    sourceFileBuff_(sourceFileBuff)
    , outFileName_(outFileName)
  {}
  virtual void callback(PeLib::PeFile32& file)
  {
    rebuildSections<32>(file, sourceFileBuff_, outFileName_);
  }
  virtual void callback(PeLib::PeFile64& file)
  {
    rebuildSections<64>(file, sourceFileBuff_, outFileName_);
  }
private:
  const std::vector<uint8_t>& sourceFileBuff_;
  const std::string& outFileName_;
};

//---------------------------------------------------------------------------------------------------

Expected<ErrorCode> SectionsPacker::ProcessExecutable(const std::string& outFileName)
{
  auto peFileOut = PeLib::openPeFile(outFileName);
  if (!peFileOut)
  {
    return Expected<ErrorCode>::fromException(std::runtime_error("openPEFile failed..."));
  }

  auto sourceFileBuff = file_utils::readFile(srcPEFile_->getFileName());

  DumpSectionsVisitor sectionsVisitor(sourceFileBuff, outFileName);
  srcPEFile_->visit(sectionsVisitor);

  return ErrorCode::ERROR_SUCC;
}

Expected<ErrorCode> SectionsPacker::IsReady() const
{
  return ErrorCode::ERROR_SUCC;
}