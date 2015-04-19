file(GLOB MainProjInc "inc/*.*")
file(GLOB MainProjSrc "src/*.*")

set(COMMON_DIRS
	${CMAKE_HOME_DIRECTORY}/common/alexandrescu
)

set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.36.0 REQUIRED program_options filesystem system)

include_directories(${BOOST_INCLUDEDIR} ${PELIB_ROOT} "inc" ${COMMON_DIRS})

add_executable(${SOLUTION_NAME} ${MainProjSrc} ${MainProjInc})

target_link_libraries(${SOLUTION_NAME} pelib ${Boost_LIBRARIES})
