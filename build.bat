@echo off
gcc src/*.c -Iinclude ^
    -IC:/msys64/mingw64/include/SDL2 ^
    -LC:/msys64/mingw64/lib ^
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_image ^
    -o game.exe ^
    -Wall
 
if %errorlevel% equ 0 (
    echo Build successful!
    game.exe
) else (
    echo Build failed!
    pause
)