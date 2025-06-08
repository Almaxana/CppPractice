cmake_minimum_required(VERSION 3.23.2)
project(labwork-11-Almaxana)

set(CMAKE_CXX_STANDARD 20)
add_executable(${CMAKE_PROJECT_NAME}_run main.cpp Algos.h Xrange.h Zip.h)

add_subdirectory(Google_tests)


set(CMAKE_CXX_FLAGS "-g")

target_link_libraries(${CMAKE_PROJECT_NAME}_run)

