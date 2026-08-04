@echo off
setlocal

set BUILD_DIR=build
set GENERATOR="Visual Studio 17 2022"
set CONFIG=Release

if exist %BUILD_DIR% (
    echo Removing old build directory...
    rmdir /s /q %BUILD_DIR%
)

mkdir %BUILD_DIR%

cmake -B %BUILD_DIR% -S . -G %GENERATOR%

echo Building project...
cmake --build %BUILD_DIR% --config %CONFIG%

if exist %BUILD_DIR%\*.sln (
    for %%f in (%BUILD_DIR%\*.sln) do start "" "%%f"
)

echo Installing to Distribution folder...
cmake --install %BUILD_DIR% --config %CONFIG% --prefix "Distribution"

echo Build finished!
endlocal
pause