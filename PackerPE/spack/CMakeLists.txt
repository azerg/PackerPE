set(expected_cmn ${common_inc}/alexandrescu)

file(GLOB_RECURSE MainProjInc "inc/*.*" "interfaces/*.*" "${CMAKE_HOME_DIRECTORY}/common/inc/*.*")
file(GLOB_RECURSE MainProjSrc "src/*.*" "${CMAKE_HOME_DIRECTORY}/common/src/*.*")

#---------------------------------------------------------------------------------
set(PRJ_DIR ${CMAKE_HOME_DIRECTORY}/spack)

set(InterfacesInc
	${PRJ_DIR}/interfaces/iimport_packer.h
	${PRJ_DIR}/interfaces/inew_pe_builder.h
	${PRJ_DIR}/interfaces/ipacker_base.h
	${PRJ_DIR}/interfaces/isections_packer.h
	${PRJ_DIR}/interfaces/istub_packer.h
	${PRJ_DIR}/interfaces/iloader_packer.h
	${PRJ_DIR}/interfaces/imain_loop.h
)
source_group("Header Files\\interfaces" FILES ${InterfacesInc})

set(CommonsInc
	${PRJ_DIR}/inc/tiny_logger.h
	${PRJ_DIR}/inc/includes.h
	${PRJ_DIR}/inc/error_defines.h
	${common_inc}/common_utils.h
	${common_inc}/file_utils.h
	${common_inc}/stub_data.h
	${expected_cmn}/expected.hpp
)
source_group("Header Files\\commons" FILES ${CommonsInc})

#---------------------------------------------------------------------------------

find_package(Boost 1.36.0 REQUIRED program_options filesystem system)

include_directories(${BOOST_ROOT} ${PELIB_ROOT} "inc" "interfaces" ${expected_cmn} ${common_inc})

add_executable(${SOLUTION_NAME} ${MainProjSrc} ${MainProjInc})

add_definitions(-DNOMINMAX) # remove nasty min max macro
target_link_libraries(${SOLUTION_NAME} pelib common ${Boost_LIBRARIES})