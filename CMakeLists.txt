cmake_minimum_required(VERSION 3.14)
project(PolynomialToolkit CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Polynomial Core Library
add_library(polynomial_lib STATIC src/Polynomial.cpp)

# Main CLI Application Executable
add_executable(PolynomialToolkit main.cpp)
target_link_libraries(PolynomialToolkit PRIVATE polynomial_lib)

# Automated Test Suite Executable
add_executable(runTests tests/test_polynomial.cpp)
target_link_libraries(runTests PRIVATE polynomial_lib)

enable_testing()
add_test(NAME AllPolynomialTests COMMAND runTests)
