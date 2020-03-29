## SPDX-License-Identifier: MIT

# Root Cmake file.

# Require CMake 3.16
cmake_minimum_required(VERSION 3.16)

# Include vcpkg toolchain file
include(cmake/vcpkg.cmake)

# Project Name and Language
project(opengl46_eg
        LANGUAGES CXX)

# Place build out to build/bin directory
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin/")

add_subdirectory(src)