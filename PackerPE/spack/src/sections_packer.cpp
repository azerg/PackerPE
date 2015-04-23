#pragma once

#include "sections_packer.h"

template<int bits>
void rebuildSections(
  PeLib::PeFile& peFileSrc
  , PeLib::PeFile& peFileOut
  , const std::vector<uint8_t>& sourceFileBuff
  , std::vector<uint8_t>& outFileBuff)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFileSrc).peHeader();
  uint32_t cbSections{};

  auto nSections = peh.calcNumberOfSections();
  for (auto idxSection = 0; idxSection < nSections; idxSection++)
  {
    peh.getSectionName(idxSection);

    auto cbSection = peh.getSizeOfRawData(idxSection);

    /*
    writeFile(
      outFileName.c_str()
      , offset
      , reinterpret_cast<char*>(&sourceFileBuff.at(peh.getPointerToRawData(idxSection)))
      , cbSection);
    */

    cbSections += cbSection;
  }

  //peh.writeSections(outFileName);
  //offset += cbSections;
}

Expected<ErrorCode> SectionsPacker::ProcessExecutable(
  const std::vector<uint8_t>& sourceFileBuff
  , std::vector<uint8_t>& outFileBuff)
{
  auto peFileOut = 
  if (!peFileOut)
  {
    return Expected<ErrorCode>::fromException(std::runtime_error("peFileOut destroyed..."));
  }

  *peFileOut = *peFile_;

  return ErrorCode::ERROR_SUCC;
}

//---------------------------------------------------------------------------------------------------

class DumpSectionsVisitor: public PeLib::PeFileVisitor
{
public:
  DumpSectionsVisitor(
    const std::vector<uint8_t>& sourceFileBuff
    , std::vector<uint8_t>& outFileBuff
    , std::shared_ptr<PeLib::PeFile> peFileOut):
    outFileBuff_(outFileBuff)
    , peFileOut_(peFileOut)
    , sourceFileBuff_(sourceFileBuff)
  {}
  virtual void callback(PeLib::PeFile32& file) { rebuildSections<32>(file, *peFileOut_, sourceFileBuff_, outFileBuff_); }
  virtual void callback(PeLib::PeFile64& file) { rebuildSections<64>(file, *peFileOut_, sourceFileBuff_, outFileBuff_); }
private:
  const std::vector<uint8_t> sourceFileBuff_;
  std::vector<uint8_t>& outFileBuff_;
  std::shared_ptr<PeLib::PeFile> peFileOut_;
};