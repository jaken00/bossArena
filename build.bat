@echo off
gcc main.c -o game.exe -lmingw32 -lSDL2main -lSDL2
if %errorlevel% equ 0 (
    echo Build successful!
    game.exe
) else (
    echo Build failed!
    pause
)