add_executable(bot main.cpp syntax_parser.cpp semantic_parser.cpp)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
