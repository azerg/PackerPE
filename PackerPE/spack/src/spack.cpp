//
#include "spack.h"

#include <cstdint>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "sections_packer.h"
#include "import_packer.h"
#include "stub_packer.h"
#include "new_pe_builder.h"
#include "file_utils.h"
#include "tiny_logger.h"

typedef std::shared_ptr<PeLib::PeFile> PeFilePtr;

template <class PeFileType>
void LoadPeFile(PeFileType& pef)
{
  pef->readMzHeader();
  pef->readPeHeader();

  pef->readExportDirectory();
  pef->readImportDirectory();
  pef->readResourceDirectory();
  pef->readExportDirectory();
  // <-- security directory ???
  pef->readRelocationsDirectory();
  pef->readDebugDirectory();
  // <-- Architecture Specific Data ???
  // <-- RVA of GP
  pef->readTlsDirectory();
  // <-- Load Config Directory ???
  pef->readBoundImportDirectory();
  pef->readIatDirectory();
  // <-- Delay Import Descriptors ???
  pef->readComHeaderDirectory();
  // <-- Reserved ???
}

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

    LoadPeFile(pef);
    //-----------------------------------------------------

    ImportPacker importPacker(pef);
    auto requiredImportDataBlocks = importPacker.GetRequiredDataBlocks();
    // moving required import blocks
    additionalSizeRequest = std::move(requiredImportDataBlocks);

    //-----------------------------------------------------

    StubPacker stubPacker(pef);
    auto stubRequiredBlocks = stubPacker.GetRequiredDataBlocks();
    std::move(stubRequiredBlocks.begin(), stubRequiredBlocks.end(), std::back_inserter(additionalSizeRequest));

    SectionsPacker sectionsPacker(pef);

    //-----------------------------------------------------
    // generating output file contents

    NewPEBuilder newPEBuilder(pef, sourceFileBuff, additionalSizeRequest, &importPacker, &stubPacker, &sectionsPacker);

    auto outBuffer = newPEBuilder.GenerateOutputPEFile().get();

    file_utils::writeFile(outFileName.c_str(), 0, reinterpret_cast<const char*>(outBuffer.data()), outBuffer.size());

    //-----------------------------------------------------

    return ErrorCode::ERROR_SUCC;
  }
  catch (std::runtime_error& err)
  {
    std::cout << err.what();
  }

  return ErrorCode::FATAL_ERROR;
}