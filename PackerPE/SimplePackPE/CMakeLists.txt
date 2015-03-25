SET(simplePackPESrc
  SimplePackPE.cpp
)

set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.36.0 REQUIRED program_options)

include_directories(${BOOST_INCLUDEDIR})

add_executable (SimplePackPE ${simplePackPESrc})

target_link_libraries (SimplePackPE pelib ${Boost_LIBRARIES})
