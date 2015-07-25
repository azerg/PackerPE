//

#include "new_pe_builder.h"

namespace pe_builder
{
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

  class DumpPeHeaderVisitor : public PeLib::PeFileVisitor
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
} // namespace pe_builder

// ---------------------------------------------------------------

Expected<std::vector<uint8_t>> NewPEBuilder::GenerateOutputPEFile()
{
  std::vector<uint8_t> outFileBuffer;
  auto offset = pe_builder::rebuildMZHeader(srcPeFile_, outFileBuffer, sourceFileBuff_);

  srcPeFile_->readPeHeader();
  pe_builder::DumpPeHeaderVisitor peVisitor(outFileBuffer, offset);
  srcPeFile_->visit(peVisitor);

  return outFileBuffer;
  //return Expected<std::vector<uint8_t>>::fromException(std::runtime_error("eek"));
}
