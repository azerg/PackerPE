#pragma once

#include "includes.h"
#include "iimport_packer.h"
#include "isections_packer.h"
#include "istub_packer.h"
#include "iloader_packer.h"

// hardcoded PackerOptions just for testing. todo(azerg): move it to main packer as external setter
enum class PackingOptions
{
  stripRelocs, // strip relocs in packed file
};
typedef std::set<PackingOptions> PackingOptionsList;

class INewPEBuilder:
  public IPackerBase
{
public:
  INewPEBuilder(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    IPackerBase(srcPEFile, PackerType::kNewPEBuilder)
  {}
  virtual Expected<std::vector<uint8_t>> GenerateOutputPEFile(
    const std::vector<uint8_t>& sourceFileBuff
    , std::vector<uint8_t>& outFileBuffer
    , ImportsArr imports_
    , std::vector<uint8_t> stubData_
    , SectionsArr newSections_
    , const PackingOptionsList& packingOptions) = 0;
  virtual ~INewPEBuilder() {};
};