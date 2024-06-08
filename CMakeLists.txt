cmake_minimum_required(VERSION 3.27)
project(debby)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_compile_options(-ggdb -pedantic-errors -D LOGGING -Wall)

include_directories("${CMAKE_SOURCE_DIR}/vendor/")

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp)

add_executable(debby ${PROJECT_SOURCES})

if (WIN32)
    target_link_libraries(debby "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2main.lib"
        "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2.lib"
        "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2_image.lib"
        "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2_ttf.lib"
        "${CMAKE_SOURCE_DIR}/libs/lua/windows/lua54.lib")
endif()

