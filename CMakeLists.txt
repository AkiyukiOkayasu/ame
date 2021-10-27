# Determine if doctest is built as a subproject (using add_subdirectory) or if it is the main project.
set(MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

if(MAIN_PROJECT)
    message("main project")
    cmake_minimum_required(VERSION 3.14)
    project(ame VERSION 0.0.11 LANGUAGES CXX)

    include(CTest)
    include(FetchContent)
    FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/onqtam/doctest GIT_TAG 2.4.5)
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

    target_compile_options(tests PUBLIC -O2 -Wall -fconstexpr-depth=2147483647)
    target_compile_features(tests PUBLIC cxx_std_17)
    target_link_libraries(tests PRIVATE doctest::doctest)
else()
    message("NOT main project")
    message("PROJECT_NAME: ${PROJECT_NAME}")
    add_library(ame INTERFACE)
    # add_library(ame::ame ALIAS ame)
    # add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
    # target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
    target_include_directories(ame INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()