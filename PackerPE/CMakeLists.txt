cmake_minimum_required(VERSION 2.8)

set(SOLUTION_NAME "spack")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# leaving only Release& Debug configurations
set(CMAKE_CONFIGURATION_TYPES Release Debug CACHE TYPE INTERNAL FORCE)

##################################################################################################################

set(common_inc ${CMAKE_HOME_DIRECTORY}/common/inc CACHE TYPE STRING)

set(BOOST_INCLUDEDIR "${BOOST_ROOT}" CACHE TYPE STRING)
set(BOOST_LIBRARYDIR "${BOOST_LIBRARYDIR}" CACHE TYPE STRING)
set(Boost_USE_STATIC_LIBS ON CACHE TYPE BOOL)

##################################################################################################################
project (${SOLUTION_NAME})

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
endif()

set(PELIB_ROOT ${CMAKE_HOME_DIRECTORY}/dependencies/pelib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

add_subdirectory (dependencies)
add_subdirectory (common)
add_subdirectory (loader)
add_subdirectory (stub2rsrc)
add_subdirectory (test/FileDump)
add_subdirectory (${SOLUTION_NAME})
