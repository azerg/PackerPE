//
#include <PeLibAux.h>
#include "common_utils.h"
#include "stub_packer.h"

class FillStubDataVisitor : public PeLib::PeFileVisitor
{
public:
  FillStubDataVisitor(stub::STUB_DATA& stubData) :
    stubData_(stubData)
  {}
  virtual void callback(PeLib::PeFile32& file) { fillStubData<32>(file); }
  virtual void callback(PeLib::PeFile64& file) { fillStubData<64>(file); }
private:
  template<int bits>
  void fillStubData(PeLib::PeFile& peFile)
  {
    const PeLib::PeHeaderT<bits>& peh = static_cast<PeLib::PeFileT<bits>&>(peFile).peHeader();

    stubData_.dwOriginalImageBase = peh.getImageBase();

    stubData_.dwOriginalEP = peh.getAddressOfEntryPoint();
    stubData_.dwOriginalRelocVA = peh.getIddBaseRelocRva();
    stubData_.dwOriginalRelocSize = peh.getIddBaseRelocSize();
    stubData_.dwOriginalImportVA = peh.getIddImportRva();
    stubData_.dwOriginalImportSize = peh.getIddImportSize();
  }

  stub::STUB_DATA& stubData_;
};

std::vector<uint8_t> StubPacker::ProcessExecutable()
{
  return InititalizeStub(); // fill stub values
}

std::vector<uint8_t> StubPacker::InititalizeStub()
{
  FillStubDataVisitor stubDataVisitor(this->stubData_);
  srcPEFile_->visit(stubDataVisitor);

  return std::vector<uint8_t>(
    reinterpret_cast<uint8_t*>(&this->stubData_),
    reinterpret_cast<uint8_t*>(&this->stubData_) + sizeof(stub::STUB_DATA));
}
