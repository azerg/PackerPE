//
#include <PeLibAux.h>
#include "common_utils.h"
#include "stub_packer.h"

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

  //utils::ReplaceContainerData(outFileBuffer, stubBlock.rawOffset, this->stubData_);
}
