set(common_inc ${CMAKE_HOME_DIRECTORY}/common/inc)

set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_USE_STATIC_LIBS ON)

file(GLOB MainProjInc "*.h" "PELib/*.h")
file(GLOB MainProjSrc "*.cpp" "PELib/*.cpp")

include_directories("inc" ${common_inc})

add_executable(stub2rsrc ${MainProjSrc} ${MainProjInc})

target_link_libraries(stub2rsrc pelib common)