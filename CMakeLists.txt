cmake_minimum_required(VERSION 3.27)
project(debby)

set(CMAKE_CXX_STANDARD 17)
set(CL_CLANG_DEBUG_FLAGS "-ggdb -pedantic-errors -D DEBUG -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion")
set(CL_CLANG_RELEASE_FLAGS "-O2 -DNDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("${CMAKE_SOURCE_DIR}/vendor/")

set(CMAKE_CXX_FLAGS ${CL_CLANG_DEBUG_FLAGS})

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp)

add_executable(debby ${PROJECT_SOURCES})

if (WIN32)
    target_link_libraries(debby "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2main.lib"
        "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2.lib"
        "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2_image.lib"
        "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2_ttf.lib"
        "${CMAKE_SOURCE_DIR}/libs/lua/windows/lua54.lib")
endif()

