//
#include "spack.h"

#include <cstdint>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "main_loop.h"
#include "sections_packer.h"
#include "import_packer.h"
#include "stub_packer.h"
#include "loader_packer.h"
#include "new_pe_builder.h"
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

ErrorCode PackExecutable(const std::string& srcFileName, const std::string& outFileName, const PackingOptionsList& packingOptions)
{
  try
  {
    if (!boost::filesystem::exists(srcFileName))
    {
      return ErrorCode::kFileNotFound;
    }

    if (boost::filesystem::exists(outFileName))
    {
      boost::filesystem::remove(outFileName);
    }

    auto pef = PeLib::openPeFile(srcFileName);
    if (!pef)
    {
      std::cout << "Invalid PE File" << std::endl;
      return ErrorCode::kInvalidPEFile;
    }

    LoadPeFile(pef);

    //----------- moving to MainLoop architecture ------------
    MainLoop mainLoop(srcFileName, outFileName);

    mainLoop.AddPacker(std::make_shared<ImportPacker>(pef));
    mainLoop.AddPacker(std::make_shared<LoaderPacker>(pef));
    mainLoop.AddPacker(std::make_shared<StubPacker>(pef));
    mainLoop.AddPacker(std::make_shared<SectionsPacker>(pef));
    mainLoop.AddPacker(std::make_shared<NewPEBuilder>(pef));

    mainLoop.PackFile();

    //-----------------------------------------------------
    // generating output file contents
    /*
    NewPEBuilder newPEBuilder(pef, sourceFileBuff, additionalSizeRequest, &importPacker, &stubPacker, &sectionsPacker, &loaderPacker, packingOptions);

    auto outBuffer = newPEBuilder.GenerateOutputPEFile().get();

    file_utils::writeFile(outFileName.c_str(), 0, reinterpret_cast<const char*>(outBuffer.data()), outBuffer.size());
    */
    //-----------------------------------------------------

    return ErrorCode::kOk;
  }
  catch (std::runtime_error& err)
  {
    std::cout << err.what();
  }

  return ErrorCode::kFatalError;
}