@echo off
setlocal enabledelayedexpansion

echo Checking Python installation...

where python >nul 2>&1
if errorlevel 1 (
    echo.
    echo ERROR: Python is not installed or not in PATH.
    echo Please install Python 3.x from https://www.python.org/downloads/
    echo.
    pause
    exit /b 1
)

echo Checking pip installation...

python -m pip --version >nul 2>&1
if errorlevel 1 (
    echo.
    echo ERROR: pip is not installed.
    echo Run: python -m ensurepip --upgrade
    echo.
    pause
    exit /b 1
)

echo Checking for jinja2...

python -m pip show jinja2 >nul 2>&1
if errorlevel 1 (
    echo jinja2 not found. Installing...
    python -m pip install jinja2 >nul 2>&1
    if errorlevel 1 (
        echo.
        echo ERROR: Failed to install jinja2.
        echo.
        pause
        exit /b 1
    )
) else (
    echo jinja2 already installed.
)

echo.
echo Building VGM Creator...

mkdir build >nul 2>&1
cd build

set LOGFILE=build_log.txt
del %LOGFILE% >nul 2>&1

echo Running CMake configure...
cmake .. > %LOGFILE% 2>&1
if errorlevel 1 goto :showError

echo Running build...
cmake --build . >> %LOGFILE% 2>&1
if errorlevel 1 goto :showError

echo Running VGM Creator...
Debug\VGMCreator.exe
goto :end

:showError
echo.
echo BUILD FAILED!
echo.
echo -------- ERROR OUTPUT --------
type %LOGFILE%
echo -------- END ERROR OUTPUT ----
echo.
pause
goto :end

:end
endlocal
