cmake_minimum_required(VERSION 3.24)
project(labwork-8-Almaxana)

set(CMAKE_CXX_STANDARD 17)
add_executable(${CMAKE_PROJECT_NAME}_run main.cpp)

add_executable(labwork-8-Almaxana main.cpp)
add_subdirectory(Google_tests)

set(CMAKE_CXX_FLAGS "-g")

target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib)