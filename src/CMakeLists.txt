## SPDX-License-Identifier: MIT

# Find dependencies
find_package(fmt REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Executable target to build
add_executable(opengl_eg)

# Source files used for opengl_eg
target_sources(opengl_eg
    PRIVATE
        main.cpp
        window.cpp
        window.h
        window_implementation.inl)

# C++ Standard to use for opengl_eg
target_compile_features(opengl_eg
    PRIVATE
        cxx_std_17)

# Provide some extra compiler options IF the compiler is MSVC
# - /W3 : enable all the warnings
# - /permissive- : tell msvc to work in conformance mode
# - /Zc:__cpluspluc : tell msvc to report C++ version correctly
# - /std:c++17 : tell msvc to use Standard C++ 17. CMake passes flag incorrectly
target_compile_options(opengl_eg
    PRIVATE 
        $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /Zc:__cplusplus /std:c++17>)

# If using MSVC, change program entry point to main()
target_link_options(opengl_eg
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/ENTRY:mainCRTStartup>)

# Define "DEBUG" if the Build type is Debug
# Define only for MSVC to silence deprecated header warnings
target_compile_definitions(opengl_eg
    PRIVATE
        UNICODE _UNICODE
        $<$<CONFIG:Debug>:DEBUG _DEBUG>
        $<$<CXX_COMPILER_ID:MSVC>:_SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING>)

# Link with dependencies
target_link_libraries(opengl_eg
    PRIVATE
        fmt::fmt
        glad::glad
        glfw
)