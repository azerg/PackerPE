//
#include "spack.h"

#include <cstdint>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "sections_packer.h"
#include "import_packer.h"
#include "new_pe_builder.h"
#include "file_utils.h"
#include "tiny_logger.h"

typedef std::shared_ptr<PeLib::PeFile> PeFilePtr;

ErrorCode PackExecutable(const std::string& srcFileName, const std::string& outFileName)
{
  try
  {
    if (!boost::filesystem::exists(srcFileName))
    {
      return ErrorCode::FILE_NOT_FOUND;
    }

    auto sourceFileBuff = file_utils::readFile(srcFileName.c_str());
    decltype(sourceFileBuff) outFileBuff;

    if (boost::filesystem::exists(outFileName))
    {
      boost::filesystem::remove(outFileName);
    }

    auto pef = PeLib::openPeFile(srcFileName);
    if (!pef)
    {
      std::cout << "Invalid PE File" << std::endl;
      return ErrorCode::INVALID_PE_FILE;
    }

    // stores data sizes required by different packers
    std::vector<RequiredDataBlock> additionalSizeRequest;

    //-----------------------------------------------------

    pef->readMzHeader();
    pef->readPeHeader();

    //-----------------------------------------------------

    ImportPacker importPacker(pef);
    additionalSizeRequest.push_back({importPacker.GetRequiredSpaceSize(), PackerTypes::kImportPacker});

    //-----------------------------------------------------

    SectionsPacker sectionsPacker(pef);
    auto sectionsArr = sectionsPacker.ProcessExecutable(sourceFileBuff, additionalSizeRequest);

    //-----------------------------------------------------

    // pick last section VA to use it as new imports VA
    auto additionalImportsBlock = std::find_if(
      sectionsArr.additionalDataBlocks.begin()
      , sectionsArr.additionalDataBlocks.end()
      ,[&](decltype(sectionsArr.additionalDataBlocks)::value_type& valIt)
    {
      return valIt.ownerType == PackerTypes::kImportPacker;
    });

    auto newImportsVA = 0;
    
    if (additionalImportsBlock != sectionsArr.additionalDataBlocks.end())
    {
      newImportsVA = additionalImportsBlock->virtualOffset;
    }

    auto importsArr = importPacker.ProcessExecutable(newImportsVA); // new import RVA passed here

    //-----------------------------------------------------
    // generating output file contents

    NewPEBuilder newPEBuilder(sectionsArr, importsArr, pef, sourceFileBuff);

    newPEBuilder.GenerateOutputPEFile();

    //-----------------------------------------------------

    return ErrorCode::ERROR_SUCC;
  }
  catch (std::runtime_error& err)
  {
    std::cout << err.what();
  }

  return ErrorCode::FATAL_ERROR;
}