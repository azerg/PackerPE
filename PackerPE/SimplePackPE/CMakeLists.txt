SET(simplePackPESrc
  SimplePackPE.cpp
)

add_executable (SimplePackPE ${simplePackPESrc})

include_directories ("${PROJECT_SOURCE_DIR}/dependencies/pelib" "${BOOST_FOLDER}")
target_link_libraries (SimplePackPE pelib)
