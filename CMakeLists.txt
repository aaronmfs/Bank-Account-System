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
# Compiler Flags
# ==============================================================================
if(MSVC)
  # Visual Studio flags (GitHub Actions uses this)
  add_compile_options(/W4)
  # Remove /WX (warnings as errors) for now to avoid blocking on warnings
else()
  # GCC/MinGW flags (for cross-compilation)
  add_compile_options(-Wall -Wextra -Wpedantic)
  # Remove -Werror for now
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
# Project Source Files
# ==============================================================================
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

# ==============================================================================
# Executable Target
# ==============================================================================
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/webview/core/include
    ${CMAKE_SOURCE_DIR}/lib/webview2/build/native/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/webview2/build/native/x64/WebView2LoaderStatic.lib
    ole32
    shell32
    shlwapi
    user32
    version
    advapi32
)
