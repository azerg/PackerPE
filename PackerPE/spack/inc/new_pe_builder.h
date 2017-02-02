#pragma once

#include "includes.h"
#include <set>
#include "inew_pe_builder.h"

class NewPEBuilder: public INewPEBuilder
{
public:
  NewPEBuilder(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    INewPEBuilder(srcPEFile)
  {}

  Expected<std::vector<uint8_t>> GenerateOutputPEFile(
    const std::vector<uint8_t>& sourceFileBuff
    , std::vector<uint8_t>& outFileBuffer
    , ImportsArr imports_
    , std::vector<uint8_t> stubData_
    , SectionsArr newSections_
    , const PackingOptionsList& packingOptions);

  Expected<ErrorCode> IsReady(const std::set<PackerType>& readyPackersList) const
  {
    std::set<PackerType> dependencies{ PackerType::kSectionsPacker, PackerType::kImportPacker, PackerType::kStubPacker };

    if (std::includes(readyPackersList.cbegin(), readyPackersList.cend(), dependencies.cbegin(), dependencies.cend()))
    {
      return ErrorCode::kOk;
    }

    return ErrorCode::kBusy;
  }
  std::vector<RequiredDataBlock> GetRequiredDataBlocks() const { return{}; }

private:
  ImportsArr imports_;
  std::vector<uint8_t> stubData_;
  SectionsArr newSections_;
  const PackingOptionsList packingOptions_;
  const std::vector<uint8_t> sourceFileBuff_;
};