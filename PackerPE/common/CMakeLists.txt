file(GLOB includes "inc/*.*" "inc/alexandrescu/*.*")
file(GLOB sources "src/*.*")

include_directories(${BOOST_ROOT} "inc")

ADD_LIBRARY(common ${sources} ${includes})
