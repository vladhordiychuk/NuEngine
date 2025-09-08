@echo off
setlocal

set BUILD_DIR=build

set GENERATOR="Visual Studio 17 2022"

if exist %BUILD_DIR% (
    echo Removing old build directory...
    rmdir /s /q %BUILD_DIR%
)

mkdir %BUILD_DIR%

cmake -B %BUILD_DIR% -S . -G %GENERATOR%

if exist %BUILD_DIR%\*.sln (
    for %%f in (%BUILD_DIR%\*.sln) do start "" "%%f"
)

endlocal
pause
