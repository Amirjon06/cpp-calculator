cmake_minimum_required(VERSION 3.15)

project(Calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -----------------------------
# Library
# -----------------------------
add_library(calc_lib STATIC
    src/calculator.cpp
)

target_include_directories(calc_lib PUBLIC
    include
)

# -----------------------------
# Main executable
# -----------------------------
add_executable(calculator
    src/main.cpp
)

target_link_libraries(calculator PRIVATE calc_lib)

# -----------------------------
# Tests
# -----------------------------
include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)

FetchContent_MakeAvailable(doctest)

enable_testing()

add_executable(tests
    tests/test_calculator.cpp
)

target_include_directories(tests PRIVATE
    ${doctest_SOURCE_DIR}/doctest
)

target_link_libraries(tests PRIVATE calc_lib)

add_test(NAME CalculatorTests COMMAND tests)