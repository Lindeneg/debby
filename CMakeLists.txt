cmake_minimum_required(VERSION 3.27)
project(debby)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-g3 -Wall -Wextra -Wconversion -Wdouble-promotion -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion -fsanitize=undefined -fsanitize-trap)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/std:c++17 /Zi /W4 /RTC1 /EHsc)
endif ()

include_directories("${CMAKE_SOURCE_DIR}/includes/")

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp)

add_executable(debby ${PROJECT_SOURCES})

if (APPLE OR UNIX)
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    target_link_libraries(debby ${SDL2_LIBRARIES}
            ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
elseif (WIN32)
    target_link_libraries(debby "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2main.lib"
            "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2.lib"
            "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2_image.lib"
            "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2_ttf.lib"
            "${CMAKE_SOURCE_DIR}/libs/lua/windows/lua54.lib")
    add_custom_command(TARGET debby POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2_image.dll"
            "${CMAKE_SOURCE_DIR}/libs/SDL2/windows/SDL2_ttf.dll"
            $<TARGET_FILE_DIR:debby>)
endif ()

add_custom_command(TARGET debby POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/")
