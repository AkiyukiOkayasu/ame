cmake_minimum_required(VERSION 3.13)
project(ame VERSION 0.1.0)

include(CTest)
include(FetchContent)

# ------------------------------------ First fetch/config doctest:
FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/onqtam/doctest GIT_TAG 2.4.5)
FetchContent_MakeAvailable(doctest)
enable_testing()
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

add_executable(tests test.cpp)
doctest_discover_tests(tests)

# Include directories
target_include_directories(tests
    PUBLIC ${PROJECT_SOURCE_DIR}
)

target_compile_options(tests PUBLIC -O2 -Wall -fconstexpr-depth=2147483647)
target_compile_features(tests PUBLIC cxx_std_17)
target_link_libraries(tests PRIVATE doctest::doctest)