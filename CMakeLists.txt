# Determine if doctest is built as a subproject (using add_subdirectory) or if it is the main project.
set(MAIN_PROJECT OFF)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

if(MAIN_PROJECT)
    message(STATUS "AME Unit test settings begin")
    cmake_minimum_required(VERSION 3.16)
    project(ame LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 20) # C++20

    include(CTest)
    include(FetchContent)
    FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/onqtam/doctest GIT_TAG v2.4.10)
    FetchContent_MakeAvailable(doctest)
    enable_testing()
    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

    add_executable(tests test/test.cpp)
    doctest_discover_tests(tests)

    # Include directories
    target_include_directories(tests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test
    )

    # add_link_options("-fuse-ld=mold") # How to use custom linker (mold, lld, gold etc..)
    target_compile_options(tests PUBLIC -O2 -Wall -fconstexpr-depth=2147483647)
    target_compile_features(tests PUBLIC cxx_std_20)
    target_link_libraries(tests PRIVATE doctest::doctest)
    message(STATUS "AME Unit test settings end")
else()
    message(STATUS "AME - C++ header-only DSP library for Cortex-M")
    add_library(ame INTERFACE)
    target_include_directories(ame INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()