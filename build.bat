@echo off
gcc main.c game.c ui.c -o game.exe ^
    -IC:/msys64/mingw64/include/SDL2 ^
    -LC:/msys64/mingw64/lib ^
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_image ^
    -Wall -Wextra
if %errorlevel% equ 0 (
    echo Build successful!
    game.exe
) else (
    echo Build failed!
    pause
)