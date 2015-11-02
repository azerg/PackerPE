#pragma once

enum class ErrorCode
{
  kOk,
  kFailed,
  kFatalError,
  kFileNotFound,
  kInvalidPEFile,
  kSectionPackError,
};