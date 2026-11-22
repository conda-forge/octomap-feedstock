cmake_minimum_required(VERSION 3.15)
project(test_octomap LANGUAGES CXX)

find_package(octomap CONFIG REQUIRED)

add_executable(test_octomap test.cpp)
target_link_libraries(test_octomap PRIVATE octomap::octomap)

enable_testing()
add_test(NAME test_octomap COMMAND test_octomap)
