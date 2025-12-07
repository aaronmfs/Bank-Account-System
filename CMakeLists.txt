cmake_minimum_required(VERSION 3.29)
project(bank_account_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall")
string(APPEND CMAKE_CXX_FLAGS " -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wpedantic")
string(APPEND CMAKE_CXX_FLAGS " -Werror")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(wxBUILD_SHARED OFF CACHE BOOL "Build wx libraries as shared libs")
add_subdirectory(lib/wxWidgets)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    wx::core
    wx::base
)

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
