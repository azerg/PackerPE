#pragma once

enum class ErrorCode
{
  kOk,
  kFailed,
  kBusy,
  kFatalError,
  kFileNotFound,
  kInvalidPEFile,
  kSectionPackError,
};