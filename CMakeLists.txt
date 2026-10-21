cmake_minimum_required(VERSION 3.20)
project(ADAS_AEB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  
URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_library(aeb ./src/vehicle.cpp ./src/ttc.cpp)
target_include_directories(aeb PUBLIC ./include)
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ./tests/*.cpp)
add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests PRIVATE GTest::gtest_main aeb)

include(GoogleTest)
gtest_discover_tests(tests) 