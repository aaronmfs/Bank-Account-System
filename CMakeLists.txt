# ==============================================================================
# Project Configuration
# ==============================================================================
# Specify the minimum CMake version required to build this project
cmake_minimum_required(VERSION 3.26)

# Define the project name
project(bank_system_sim)

# ==============================================================================
# C++ Standard Settings
# ==============================================================================
# Set the C++ language standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Require the specified C++ standard (do not allow fallback)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions (use strict ISO C++)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================================================================
# MSVC Runtime Library Configuration
# ==============================================================================
# Use the static multithreaded runtime library in Debug configuration
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# ==============================================================================
# Compiler Flags
# ==============================================================================
# Check if the compiler is Microsoft Visual C++
if(MSVC)
    # Add high warning level and treat warnings as errors for MSVC
    add_compile_options(/W4 /WX)
else()
    # Enable common warnings, extra warnings, pedantic mode, and treat warnings as errors
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ==============================================================================
# Build Configuration
# ==============================================================================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ==============================================================================
# Third-party Libraries
# ==============================================================================
# Add profiler library subdirectory
add_subdirectory("lib/httplib")

# ==============================================================================
# Source Files
# ==============================================================================
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# ==============================================================================
# Executable Target
# ==============================================================================
add_executable(${PROJECT_NAME} ${SOURCES})

# ==============================================================================
# Target Properties
# ==============================================================================
# Release only: embed relative path to resources (portable / for release)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    RESOURCES_PATH="./assets/"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets to build directory..."
)

# Development only: embed absolute path to resources (not portable / not for release)
# target_compile_definitions(${PROJECT_NAME} PUBLIC
#     RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
# )

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    httplib
)


# ==============================================================================
# Windows-specific Configuration
# ==============================================================================
if(WIN32)

    # Optionally mark the application as a GUI app (no console window)
    # set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ole32
        shell32
        shlwapi
        user32
        version
        advapi32
    )

endif()
