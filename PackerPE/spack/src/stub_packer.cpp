//
#include <PeLibAux.h>
#include "common_utils.h"
#include "stub_packer.h"

class FillStubDataVisitor : public PeLib::PeFileVisitor
{
public:
  FillStubDataVisitor(
    std::vector<uint8_t>& outFileBuffer
    , const AdditionalDataBlocksType& additionalDataBlocks
    , stub::STUB_DATA& stubData) :
    outFileBuffer_(outFileBuffer)
    , additionalDataBlocks_(additionalDataBlocks)
    , stubData_(stubData)
  {}
  virtual void callback(PeLib::PeFile32& file) { fillStubData<32>(file); }
  virtual void callback(PeLib::PeFile64& file) { fillStubData<64>(file); }
private:
  template<int bits>
  void fillStubData(PeLib::PeFile& peFile)
  {
    const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

    stubData_.dwOriginalEP = peh.getAddressOfEntryPoint();
    stubData_.dwOriginalIVA = peh.getIddImportRva();
    stubData_.dwOriginalIS = peh.getIddImportSize();
  }

  std::vector<uint8_t>& outFileBuffer_;
  const AdditionalDataBlocksType& additionalDataBlocks_;
  stub::STUB_DATA& stubData_;
};

void StubPacker::ProcessExecutable(
  std::vector<uint8_t>& outFileBuffer
  , const AdditionalDataBlocksType& additionalDataBlocks)
{
  AdditionalDataBlocksType preallocatedBlocks;
  std::copy_if(
    additionalDataBlocks.cbegin()
    , additionalDataBlocks.cend()
    , std::back_inserter(preallocatedBlocks)
    , [](const auto& block)->auto
  {
    return block.ownerType == PackerType::kStubPacker;
  });

  assert(preallocatedBlocks.size() == 1);

  auto stubBlock = preallocatedBlocks.front();

  auto stubDataVt = InititalizeStub(outFileBuffer, additionalDataBlocks); // fill stub values
  
  utils::ReplaceContainerData(outFileBuffer, stubBlock.rawOffset, stubDataVt);
}

std::vector<uint8_t> StubPacker::InititalizeStub(
  std::vector<uint8_t>& sourceFileBuff
  , const AdditionalDataBlocksType& additionalDataBlocks)
{
  FillStubDataVisitor stubDataVisitor(sourceFileBuff, additionalDataBlocks, this->stubData_);
  srcPEFile_->visit(stubDataVisitor);

  return std::vector<uint8_t>(
    reinterpret_cast<uint8_t*>(&this->stubData_),
    reinterpret_cast<uint8_t*>(&this->stubData_) + sizeof(stub::STUB_DATA));
}
