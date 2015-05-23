<<<<<<< HEAD
add_executable(bot main.cpp syntax_parser.cpp semantic_parser.cpp)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
=======
add_library(server_api server_api.cpp)

add_executable(main main.cpp syntax_parser.cpp)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread" )


>>>>>>> d9a6ce2d81e6f285d2a4966cf685fad09db95f06
