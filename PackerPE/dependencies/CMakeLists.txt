# source files for pelib
SET(pelib_src
  pelib/BoundImportDirectory.cpp
  pelib/ComHeaderDirectory.cpp
  pelib/DebugDirectory.cpp
  pelib/ExportDirectory.cpp
  pelib/IatDirectory.cpp
  pelib/MzHeader.cpp
  pelib/PeFile.cpp
  pelib/PeHeader.cpp
  pelib/PeLibAux.cpp
  pelib/RelocationsDirectory.cpp
  pelib/ResourceDirectory.cpp
  pelib/buffer/InputBuffer.cpp
  pelib/buffer/OutputBuffer.cpp
)

SET(pelib_headers
  pelib/BoundImportDirectory.h
  pelib/ComHeaderDirectory.h
  pelib/DebugDirectory.h
  pelib/ExportDirectory.h
  pelib/IatDirectory.h
  pelib/MzHeader.h
  pelib/PeFile.h
  pelib/PeHeader.h
  pelib/PeLibAux.h
  pelib/RelocationsDirectory.h
  pelib/ResourceDirectory.h
  pelib/buffer/InputBuffer.h
  pelib/buffer/OutputBuffer.h
  pelib/PeLibInc.h
  pelib/TlsDirectory.h
)

ADD_LIBRARY(pelib ${pelib_src} ${pelib_headers})
