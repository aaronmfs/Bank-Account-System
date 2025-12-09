cmake_minimum_required(VERSION 3.22)
project(bank_account_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Cross-compilation setup for MinGW
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Tell CMake not to test the compiler (avoids cross-compilation issues)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

string(APPEND CMAKE_CXX_FLAGS " -Wall")
string(APPEND CMAKE_CXX_FLAGS " -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wpedantic")
string(APPEND CMAKE_CXX_FLAGS " -Werror")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Manual wxWidgets setup for prebuilt binaries
set(WX_ROOT ${CMAKE_SOURCE_DIR}/lib/wxWidgets)
set(WX_LIB ${WX_ROOT}/lib/gcc810_x64_dll)

include_directories(${WX_ROOT}/include ${WX_LIB}/mswu)
link_directories(${WX_LIB})

# wxWidgets libraries
set(WX_LIBS wxmsw32u_core wxbase32u wxpng wxzlib wxregexu wxexpat)

# Windows system libraries
set(WIN_LIBS -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool
    -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -ladvapi32)

add_definitions(-D__WXMSW__ -DUNICODE -D_UNICODE)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${WX_LIBS} ${WIN_LIBS})

# Example
# Optional: Link with Boost (static)
# Uncomment if using Boost
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
# find_package(Boost REQUIRED COMPONENTS filesystem system)
# target_link_libraries(${PROJECT_NAME}
#     Boost::filesystem
#     Boost::system
# )

# Optional: Enable testing
# enable_testing()
# add_subdirectory(tests)
