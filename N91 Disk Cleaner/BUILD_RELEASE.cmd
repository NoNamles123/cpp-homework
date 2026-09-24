@echo off
setlocal EnableExtensions EnableDelayedExpansion
chcp 65001 >nul

set "CMAKE_EXE="
where cmake >nul 2>&1 && set "CMAKE_EXE=cmake"

if not defined CMAKE_EXE (
    set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    if exist "!VSWHERE!" (
        for /f "usebackq delims=" %%I in (`"!VSWHERE!" -latest -products * -property installationPath`) do set "VSROOT=%%I"
        if defined VSROOT if exist "!VSROOT!\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
            set "CMAKE_EXE=!VSROOT!\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
        )
    )
)

if not defined CMAKE_EXE (
    echo CMake was not found.
    echo Install Visual Studio with "Desktop development with C++"
    echo and CMake tools for Windows.
    pause
    exit /b 1
)

if not exist build-n91 mkdir build-n91
"%CMAKE_EXE%" -S . -B build-n91 -A x64
if errorlevel 1 goto :error

"%CMAKE_EXE%" --build build-n91 --config Release
if errorlevel 1 goto :error

echo.
echo Build complete:
echo build-n91\Release\N91DiskCleaner.exe
pause
exit /b 0

:error
echo.
echo Build failed.
pause
exit /b 1
