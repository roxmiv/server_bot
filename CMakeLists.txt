add_library(server_api server_api.cpp)

add_executable(main main.cpp syntax_parser.cpp)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread" )


