set(expected_cmn ${common_inc}/alexandrescu)

file(GLOB MainProjInc "inc/*.*" "${CMAKE_HOME_DIRECTORY}/common/inc/*.*")
file(GLOB MainProjSrc "src/*.*" "${CMAKE_HOME_DIRECTORY}/common/src/*.*")

find_package(Boost 1.36.0 REQUIRED program_options filesystem system)

include_directories(${BOOST_INCLUDEDIR} ${PELIB_ROOT} "inc" ${expected_cmn} ${common_inc})

add_executable(${SOLUTION_NAME} ${MainProjSrc} ${MainProjInc})

add_definitions(-DNOMINMAX) # remove nasty min max macro
target_link_libraries(${SOLUTION_NAME} pelib common ${Boost_LIBRARIES})