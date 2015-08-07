set(PROJECT_NAME "loader_x86")
set(common_inc ${CMAKE_HOME_DIRECTORY}/common/inc)

set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_USE_STATIC_LIBS ON)

file(GLOB MainProjInc "inc/*.*" ${common_inc}/stub_data.h)
file(GLOB MainProjSrc "src/*.*")

include_directories("inc" ${common_inc})

add_executable(${PROJECT_NAME} ${MainProjSrc} ${MainProjInc})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Loaders)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:StubEP /DYNAMICBASE:NO /NXCOMPAT:NO /NODEFAULTLIB /SAFESEH:NO")
# enable all optimizations + remove all debug stuff ( just dont know how to switch off Debug build for single prj in cmake )
string(REPLACE "/D_DEBUG" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REPLACE "/MDd" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})  # no additional libs
string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})  # for release too ;)
string(REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})      # no debug info
string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})    # no runtime checks
string(REPLACE "/Ob0" "/Ob2" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}) # inline as much as possible
string(REPLACE "/Od" "/Ox" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})   # full optimization instead of none

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/EHa /GR-")

target_link_libraries(${PROJECT_NAME} common)