#pragma once

#include "includes.h"
#include "inew_pe_builder.h"

class NewPEBuilder: INewPEBuilder
{
public:
  NewPEBuilder(
    PeFilePtr & srcPeFile
    , const std::vector<uint8_t>& sourceFileBuff
    , const std::vector<RequiredDataBlock>& additionalSizeRequest
    , IImportPacker* pImportPacker
    , IStubPacker* pStubPacker
    , ISectionsPacker* pSectionsPacker);

  Expected<std::vector<uint8_t>> GenerateOutputPEFile();

private:
  IImportPacker* importPacker_;
  IStubPacker* stubPacker_;
  ISectionsPacker* sectionsPacker_;
  const std::vector<uint8_t>& sourceFileBuff_;
  const std::vector<RequiredDataBlock>& additionalSizeRequest_;
  PeFilePtr& srcPeFile_;
  SectionsArr newSections_;
  ImportsArr newImports_;
};