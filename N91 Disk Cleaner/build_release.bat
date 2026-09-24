@echo off
setlocal
cd /d "%~dp0"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [ERROR] vswhere.exe not found. Install Visual Studio / Build Tools with Desktop development with C++.
    pause
    exit /b 1
)

set "MSBUILD="
for /f "usebackq tokens=*" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do set "MSBUILD=%%I"

if not defined MSBUILD (
    echo [ERROR] MSBuild not found.
    pause
    exit /b 1
)

if not exist "N91DiskCleaner.sln" (
    echo [ERROR] Put this BAT next to N91DiskCleaner.sln.
    pause
    exit /b 1
)

echo Using: %MSBUILD%
echo.
"%MSBUILD%" "N91DiskCleaner.sln" /m /t:Rebuild /p:Configuration=Release /p:Platform=x64

if errorlevel 1 (
    echo.
    echo [BUILD FAILED] Copy the FIRST compiler error, not the last 40 follow-up errors.
    pause
    exit /b 1
)

echo.
echo [OK] Release build finished.
if exist "x64\Release\N91DiskCleaner.exe" echo EXE: %CD%\x64\Release\N91DiskCleaner.exe
pause
