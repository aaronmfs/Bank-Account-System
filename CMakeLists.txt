# ==============================================================================
# Project Configuration
# ==============================================================================
cmake_minimum_required(VERSION 3.22)
project(bank_account_system)

# ==============================================================================
# C++ Standard Settings
# ==============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================================================================
# MSVC Runtime Library Configuration
# ==============================================================================
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  # set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
endif()

# ==============================================================================
# Compiler Flags
# ==============================================================================
if(MSVC)
  # Visual Studio flags
  add_compile_options(/W4)
else()
  # GCC/MinGW flags
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ==============================================================================
# Build Configuration
# ==============================================================================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ==============================================================================
# Third-party Libraries
# ==============================================================================
# add_subdirectory("lib/profilerLib")
# add_subdirectory("lib/safeSave")
# Shits needs cmake inside them
add_subdirectory("lib/webview")

# ==============================================================================
# Source Files
# ==============================================================================
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# ==============================================================================
# Executable Target
# ==============================================================================
add_executable(${PROJECT_NAME} ${SOURCES})

# ==============================================================================
# Target Properties
# ==============================================================================
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

# ==============================================================================
# Windows-specific Configuration
# ==============================================================================
if(WIN32)
  # Uncomment for GUI application (no console window)
  # set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

  # Windows libraries
  target_link_libraries(${PROJECT_NAME} PRIVATE
        ole32
        shell32
        shlwapi
        user32
        version
        advapi32
        webview::core
    )
endif()

# ==============================================================================
# Optional: External Libraries
# ==============================================================================
# Add your library configurations here
# Example:
# target_include_directories(${PROJECT_NAME} PRIVATE
#     ${CMAKE_SOURCE_DIR}/lib/some_library/include
# )
#
# target_link_libraries(${PROJECT_NAME} PRIVATE
#     ${CMAKE_SOURCE_DIR}/lib/some_library/lib/library.lib
# )
