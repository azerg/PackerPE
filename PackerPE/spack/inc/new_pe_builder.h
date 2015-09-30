#pragma once

#include "includes.h"
#include <set>
#include "inew_pe_builder.h"

// hardcoded PackerOptions just for testing. todo(azerg): move it to main packer as external setter
enum class PackingOptions
{
  stripRelocs, // strip relocs in packed file
};
typedef std::set<PackingOptions> PackingOptionsList;

class NewPEBuilder: INewPEBuilder
{
public:
  NewPEBuilder(
    PeFilePtr & srcPeFile
    , const std::vector<uint8_t>& sourceFileBuff
    , const std::vector<RequiredDataBlock>& additionalSizeRequest
    , IImportPacker* pImportPacker
    , IStubPacker* pStubPacker
    , ISectionsPacker* pSectionsPacker
    , ILoaderPacker* pLoaderPacker
    , const PackingOptionsList& packingOptions);

  Expected<std::vector<uint8_t>> GenerateOutputPEFile();

private:
  IImportPacker* importPacker_;
  IStubPacker* stubPacker_;
  ISectionsPacker* sectionsPacker_;
  ILoaderPacker* loaderPacker_;
  const PackingOptionsList& packingOptions_;
  const std::vector<uint8_t>& sourceFileBuff_;
  const std::vector<RequiredDataBlock>& additionalSizeRequest_;
  PeFilePtr& srcPeFile_;
  SectionsArr newSections_;
  ImportsArr newImports_;
};