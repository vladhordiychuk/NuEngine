@echo off

echo [NBT] Setting up Windows build environment...

CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if %errorlevel% neq 0 (
    echo ERROR: Could not find vcvars64.bat.
    echo Please check the path to your Visual Studio installation.
    pause
    exit /b 1
)

echo [NBT] Environment ready. Compiling...

if not exist "build" mkdir build

cl.exe ^
    /nologo ^
    /EHsc ^
    /std:c++20 ^
    /Fe:build/nbt.exe ^
    src\main.cpp ^
    src\Core\BuildSystem.cpp ^
    src\Core\Module.cpp ^
    src\Generators\VSGenerator.cpp ^
    src\Generators\MakefileGenerator.cpp ^
    src\Generators\NinjaGenerator.cpp ^
    /I include ^
    /I thirdparty/toml11/include ^
    /link /SUBSYSTEM:CONSOLE

if %errorlevel% neq 0 (
    echo ERROR: NBT compilation failed.
    pause
    exit /b 1
)

echo.
echo ==============================================
echo  NBT built successfully: build\nbt.exe
echo ==============================================
echo.
echo Now run:
echo   build\nbt.exe build ..\Game\Game.nuproject
echo.