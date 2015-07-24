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

uint32_t rebuildMZHeader(PeFilePtr& peFile, std::vector<uint8_t>& outFileBuffer, std::vector<uint8_t>& sourceFileBuff)
{
  std::vector<PeLib::byte> mzHeadBuffer;
  auto mzHead = peFile->mzHeader();
  mzHead.rebuild(mzHeadBuffer);

  std::copy(mzHeadBuffer.cbegin(), mzHeadBuffer.cend(), std::back_inserter(outFileBuffer));

  // append mzHeader with undocumented data located between MZHeader & PEHeader
  auto peOffset = mzHead.getAddressOfPeHeader();
  auto cbMZHead = mzHead.size();
  auto cbJunkData = std::max(peOffset - cbMZHead, static_cast<uint32_t>(0));
  if (cbJunkData)
  {
    std::copy(&sourceFileBuff.at(cbMZHead), &sourceFileBuff.at(cbMZHead + cbJunkData), std::back_inserter(outFileBuffer));
  }

  return cbMZHead + cbJunkData;
}

template<int bits>
void rebuildPEHeader(PeLib::PeFile& peFile, std::vector<uint8_t>& outFileBuffer, uint32_t& offset)
{
  const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

  std::vector<PeLib::byte> peHeaderBuff;
  peh.rebuild(peHeaderBuff);

  PeLib::PeHeaderT<bits> pehOut;
  auto cbAfterHeader = peh.size();
  std::copy(peHeaderBuff.cbegin(), peHeaderBuff.cend(), std::back_inserter(outFileBuffer));

  offset += pehOut.size();

  // append with aligned zero-bytes
  auto headersSize = peh.getSizeOfHeaders() - offset;
  if (headersSize)
  {
    //todo(azerg): strip unused data by fixing size of headers ?
    outFileBuffer.insert(outFileBuffer.end(), headersSize, 0);
  }
  offset += headersSize;
}

//------------------------------------------------------------------------

class DumpPeHeaderVisitor: public PeLib::PeFileVisitor
{
public:
  DumpPeHeaderVisitor(std::vector<uint8_t>& outFileBuffer, uint32_t& offset) :
    outFileBuffer_(outFileBuffer)
    , offset_(offset)
  {}
  virtual void callback(PeLib::PeFile32& file) { rebuildPEHeader<32>(file, outFileBuffer_, offset_); }
  virtual void callback(PeLib::PeFile64& file) { rebuildPEHeader<64>(file, outFileBuffer_, offset_); }
private:
  std::vector<uint8_t>& outFileBuffer_;
  uint32_t& offset_;
};

//------------------------------------------------------------------------

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

    std::vector<uint8_t> outFileBuffer;

    pef->readMzHeader();
    auto offset = rebuildMZHeader(pef, outFileBuffer, sourceFileBuff);

    // stores data sizes required by different packers
    std::vector<RequiredDataBlock> additionalSizeRequest;

    //-----------------------------------------------------

    pef->readPeHeader();
    DumpPeHeaderVisitor peVisitor(outFileBuffer, offset);
    pef->visit(peVisitor);

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

    NewPEBuilder newPEBuilder(sectionsArr, importsArr, pef);

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