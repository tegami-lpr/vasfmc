cmake_minimum_required(VERSION 3.12.0)

project(vasfmc-project)

set(VF_BIN_DIR "bin" CACHE STRING "Binaries directory")
set(VF_INCLUDE_DIR "include" CACHE STRING "Includes directory")
set(VF_LIB_DIR "lib" CACHE STRING "Libraries directory")
set(VF_CONFIG_DIR "share/cmake" CACHE STRING "CMake config directory")

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")

add_subdirectory(vaslib)
add_subdirectory(vasfmc)
