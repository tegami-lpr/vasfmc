cmake_minimum_required(VERSION 3.12.0)

project(fwlibtests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB FWLT_SOURCES main.cpp)

add_executable(fwlibtest ${FWLT_SOURCES})
target_link_libraries(fwlibtest fwlib)

add_test(NAME fwlibtest COMMAND fwlibtest)