#pragma once

#include <memory>
#include <cstdint>

#pragma warning( push )
#pragma warning( disable : 4018 244 996)
#include "PeLib.h"
typedef std::shared_ptr<PeLib::PeFile> PeFilePtr;
#pragma warning( pop )

#include "expected.hpp"
#include "error_defines.h"