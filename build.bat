@echo off
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0"

where py >nul 2>&1
if not errorlevel 1 (
    py -3 tools\build_tui\bootstrap.py
    set "EXIT_CODE=!ERRORLEVEL!"
    if not "!EXIT_CODE!"=="0" pause
    exit /b !EXIT_CODE!
)

where python >nul 2>&1
if not errorlevel 1 (
    python tools\build_tui\bootstrap.py
    set "EXIT_CODE=!ERRORLEVEL!"
    if not "!EXIT_CODE!"=="0" pause
    exit /b !EXIT_CODE!
)

echo Python 3.9 or newer was not found.
echo Install Python from https://www.python.org/downloads/ and try again.
pause
exit /b 1
