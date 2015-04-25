# source files for pelib

file(GLOB pelib_src "pelib/*.cpp" "pelib/buffer/*.cpp")
file(GLOB pelib_headers "pelib/*.h" "pelib/buffer/*.h")

ADD_LIBRARY(pelib ${pelib_src} ${pelib_headers})
