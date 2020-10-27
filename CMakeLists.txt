cmake_minimum_required(VERSION 3.0.0)
project(ametest VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 REQUIRED)
add_executable(tests test.cpp)
target_link_libraries(tests Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)
