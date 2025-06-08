cmake_minimum_required(VERSION 3.24)
project(labwork-9-Almaxana)

set(CMAKE_CXX_STANDARD 17)
add_executable(${CMAKE_PROJECT_NAME}_run main.cpp)

add_executable(labwork-9-Almaxana main.cpp)

set(CMAKE_CXX_FLAGS "-g")

target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib)