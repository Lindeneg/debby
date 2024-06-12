@echo off

:: Uses CL compiler:
:: https://learn.microsoft.com/en-us/cpp/build/reference/compiler-command-line-syntax

SETLOCAL

SET OBJ_DIR=obj
SET OUTPUT_DIR=build\bin

:: Remove previous build files
@RD /S /Q ".\%OBJ_DIR%"
@RD /S /Q ".\%OUTPUT_DIR%"

MKDIR %OBJ_DIR%
MKDIR %OUTPUT_DIR%

SET SDL2_LIB=.\libs\SDL2\windows
SET LUA_LIB=.\libs\lua\windows

:: Set source files
SET SRC_FILES=src\main.cpp ^
    src\ecs\*.cpp ^
    src\managers\*.cpp

:: Set compiler options
SET COMPILER_FLAGS=/std:c++17 /Zi /W4 /RTC1 /EHsc /Fo%OBJ_DIR%\ /Fd"%OUTPUT_DIR%\vc140.pdb" /Fe"%OUTPUT_DIR%\debby.exe"

SET COMPILER_INCLUDE=/I .\includes

SET COMPILER_LINK=/link "%SDL2_LIB%\SDL2main.lib" "%SDL2_LIB%\SDL2.lib" "%SDL2_LIB%\SDL2_image.lib" "%SDL2_LIB%\SDL2_ttf.lib" "%LUA_LIB%\lua54.lib"

:: Compile the program
CL %COMPILER_INCLUDE% %SRC_FILES% %COMPILER_FLAGS% %COMPILER_LINK%

:: Copy SDL2 DLL files
COPY /Y "%SDL2_LIB%\*.dll" ".\%OUTPUT_DIR%\"

ENDLOCAL

