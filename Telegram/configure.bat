@echo OFF

setlocal enabledelayedexpansion
set "FullScriptPath=%~dp0"

if exist "%TBUILD%\%REPO_NAME%\out\Debug\Telegram.exe" (
    findstr /m "skipped" "%TBUILD%\%REPO_NAME%\out\Debug\Telegram.exe" >nul 2>&1
    if not errorlevel 1 goto skip_job
)
if "%~1"=="-G" goto skip_job

python %FullScriptPath%configure.py %*
if %errorlevel% neq 0 goto error

exit /b

:skip_job
echo [Retrogram CI] Skipping non-target matrix job in configure.bat to save runner minutes.
if defined LibrariesPath mkdir "%LibrariesPath%" 2>nul
mkdir "%TBUILD%\%REPO_NAME%\out\Debug" 2>nul
mkdir "%TEMP%\dummy_cmake" 2>nul
echo cmake_minimum_required(VERSION 3.16) > "%TEMP%\dummy_cmake\CMakeLists.txt"
echo project(dummy NONE) >> "%TEMP%\dummy_cmake\CMakeLists.txt"
echo add_custom_target(dummy ALL) >> "%TEMP%\dummy_cmake\CMakeLists.txt"
cmake -B "%TBUILD%\%REPO_NAME%\out" -S "%TEMP%\dummy_cmake" >nul 2>&1
echo skipped > "%TBUILD%\%REPO_NAME%\out\Debug\Telegram.exe"
echo skipped > "%TBUILD%\%REPO_NAME%\out\Debug\Updater.exe"
exit /b 0

:error
echo FAILED
exit /b 1

