cmake_minimum_required(VERSION 3.0.0)
project(ametest VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O1 -Wall -Wconversion -Wunsuffixed-float-constants")

# 単体テスト用define
add_compile_options(-D UNIT_TEST)

# Include directories
set(INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/CMSIS_5/CMSIS/DSP/Include
)
include_directories("${INCLUDE_DIR}")

find_package(Catch2 REQUIRED)
add_executable(tests test.cpp)
target_link_libraries(tests Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)
