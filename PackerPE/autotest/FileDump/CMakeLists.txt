SET(FileDumpSrc
  FileDump.cpp
)

include_directories(${PELIB_ROOT})

add_executable (FileDumpTest ${FileDumpSrc})

target_link_libraries (FileDumpTest pelib)
