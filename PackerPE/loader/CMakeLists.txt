set(PROJECT_NAME "loader_x86")

file(GLOB MainProjInc "inc/*.*" ${common_inc}/stub_data.h)
file(GLOB MainProjSrc "src/*.*")

include_directories("inc" ${common_inc})

add_executable(${PROJECT_NAME} ${MainProjSrc} ${MainProjInc})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Loaders)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
"/ENTRY:StubEP /DYNAMICBASE:NO /NXCOMPAT:NO /NODEFAULTLIB /SAFESEH:NO /INCREMENTAL:NO")
# enable all optimizations + remove all debug stuff ( just dont know how to switch off Debug build for single prj in cmake )
string(REPLACE "/D_DEBUG" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
string(REPLACE "/MDd" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})  # no additional libs
string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})  # for release too ;)
string(REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})      # no debug info
string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})    # no runtime checks
string(REPLACE "/Ob0" "/Ob2" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}) # inline as much as possible
string(REPLACE "/Od" "/Ox" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})   # full optimization instead of none
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/EHa /GR-")

add_definitions(-DNDEBUG)  # no debug flag
string(REPLACE "/debug" "" CMAKE_EXE_LINKER_FLAGS_DEBUG ${CMAKE_EXE_LINKER_FLAGS_DEBUG})        # removing debug info from Debug build!!
string(REPLACE "/debug" "" CMAKE_SHARED_LINKER_FLAGS_DEBUG ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})  # removing debug info from Debug build!!

target_link_libraries(${PROJECT_NAME} common)