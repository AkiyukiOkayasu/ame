cmake_minimum_required(VERSION 3.8)
project(ame VERSION 0.1.0)

# Include directories
set(INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}
)
include_directories("${INCLUDE_DIR}")

# Catch2
find_package(Catch2 REQUIRED)
add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)

target_compile_options(tests PUBLIC -O2 -Wall)
target_compile_features(tests PUBLIC cxx_std_17)
# target_compile_definitions(tests )マクロ設定
