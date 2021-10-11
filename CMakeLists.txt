cmake_minimum_required(VERSION 3.13)
project(ame VERSION 0.1.0)

include(CTest)
include(FetchContent)
# ------------------------------------ First fetch/config doctest:
FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/onqtam/doctest GIT_TAG 2.3.7)
FetchContent_MakeAvailable(doctest)
enable_testing()
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

add_executable(tests test.cpp)
doctest_discover_tests(tests)


# Include directories
target_include_directories(tests
    PUBLIC ${PROJECT_SOURCE_DIR}    
)

# Automatic test registration with Catch2 v3 using FetchContent
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#automatic-test-registration
# list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

target_compile_options(tests PUBLIC -O2 -Wall -fconstexpr-depth=2147483647 -fconstexpr-steps=2147483647)
target_compile_features(tests PUBLIC cxx_std_17)
target_link_libraries(tests PRIVATE doctest::doctest)
