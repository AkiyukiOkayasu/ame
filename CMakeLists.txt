cmake_minimum_required(VERSION 3.13)
project(ame VERSION 0.1.0)

# Catch2
find_package(Catch2 REQUIRED)
add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2)

# Include directories
target_include_directories(tests
    PUBLIC ${PROJECT_SOURCE_DIR}
)

include(CTest)
include(Catch)
catch_discover_tests(tests)

target_compile_options(tests PUBLIC -O2 -Wall -fconstexpr-depth=2147483647 -fconstexpr-steps=2147483647)
target_compile_features(tests PUBLIC cxx_std_17)
# target_compile_definitions(tests )マクロ設定
