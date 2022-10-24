cmake_minimum_required(VERSION 3.23)
project(FILE_TERROR)

set(CMAKE_CXX_STANDARD 23)

add_executable(FILE_TERROR main.cpp FolderTerrorist/FolderTerrorist.cpp FolderTerrorist/FolderTerrorist.h)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

SET(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(FolderTerrorist)
target_link_libraries(FILE_TERROR FolderTerroristLib)