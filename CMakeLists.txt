cmake_minimum_required(VERSION 3.23)
project(FILE_TERROR)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 23)
set(TEST_NAME ${PROJECT_NAME}_tests)
add_executable(${TEST_NAME} test.cpp FolderTerrorist/FolderTerrorist.cpp FolderTerrorist/FolderTerrorist.h)

target_link_libraries(${TEST_NAME} gtest_main)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

SET(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(FolderTerrorist)
target_link_libraries(${TEST_NAME} FolderTerroristLib)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})