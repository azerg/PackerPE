set(PROJECT_NAME "loader_x86")
set(common_inc ${CMAKE_HOME_DIRECTORY}/common/inc)

set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_USE_STATIC_LIBS ON)

file(GLOB MainProjInc "inc/*.*")
file(GLOB MainProjSrc "src/*.*")

include_directories("inc" ${common_inc})

add_executable(${PROJECT_NAME} ${MainProjSrc} ${MainProjInc})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Loaders)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:StubEP /DYNAMICBASE:NO /NXCOMPAT:NO /NODEFAULTLIB /SAFESEH:NO")

target_link_libraries(${PROJECT_NAME} common)