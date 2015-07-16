#pragma once

#pragma warning( push )
#pragma warning( disable : 4018 244 996)
#include "PeLib.h"
#pragma warning( pop ) 

#include "iimport_packer.h"

struct ImportEntry
{
  //PELIB_IMAGE_IMPORT_DIRECTORY
};

typedef std::vector<int> ImportsArr;

class ImportPacker : IImportPacker
{
public:
  ImportPacker(std::shared_ptr<PeLib::PeFile>& srcPEFile) :
    IImportPacker(srcPEFile)
  {}
  virtual ImportsArr ProcessExecutable();
  virtual Expected<ErrorCode> IsReady() const { return ErrorCode::ERROR_SUCC; }
private:
  uint32_t GetSectionsOffset() const;
};