#pragma once

#include <string>
#include "error_defines.h"
#include "new_pe_builder.h"

ErrorCode PackExecutable(const std::string& srcFileName, const std::string& outFileName, const PackingOptionsList& packingOptions);