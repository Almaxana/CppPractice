cmake_minimum_required(VERSION 3.24)
project(labwork_12_Almaxana)

set(CMAKE_CXX_STANDARD 17)

add_executable(${CMAKE_PROJECT_NAME}_run main.cpp ParserSQL.cpp Options.cpp ParserTree.cpp)
add_subdirectory(Google_tests)

set(CMAKE_CXX_FLAGS "-g")

target_link_libraries(${CMAKE_PROJECT_NAME}_run)