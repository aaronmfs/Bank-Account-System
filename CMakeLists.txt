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
# Cross-Compilation Setup (MinGW for Windows)
# ==============================================================================
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Skip compiler testing (avoids cross-compilation issues)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# ==============================================================================
# Compiler Flags
# ==============================================================================
# Warning flags
string(APPEND CMAKE_CXX_FLAGS " -Wall")
string(APPEND CMAKE_CXX_FLAGS " -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wpedantic")
string(APPEND CMAKE_CXX_FLAGS " -Werror")
# string(APPEND CMAKE_CXX_FLAGS " -static")

# Static linking flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

# ==============================================================================
# Build Configuration
# ==============================================================================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ==============================================================================
# Windows System Libraries
# ==============================================================================
set(WIN_LIBS
    -lkernel32
    -luser32
    -lgdi32
    -lcomdlg32
    -lwinspool
    -lshell32
    -lole32
    -loleaut32
    -luuid
    -lcomctl32
    -ladvapi32
)

# ==============================================================================
# Project Source Files
# ==============================================================================
include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# ==============================================================================
# Executable Target
# ==============================================================================
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME})
