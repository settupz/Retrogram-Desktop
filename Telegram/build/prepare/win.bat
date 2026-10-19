@echo OFF

set "FullScriptPath=%~dp0"

if not "%~3"=="" goto skip_job
if defined LibrariesPath (
    echo "%LibrariesPath%" | findstr /i "win64" >nul
    if errorlevel 1 goto skip_job
)

echo [Retrogram CI] Freeing disk space on drive C: before library build...
rmdir /s /q "C:\Program Files\Android" 2>nul
rmdir /s /q "C:\Program Files (x86)\Android" 2>nul
rmdir /s /q "C:\hostedtoolcache\windows\Ruby" 2>nul
rmdir /s /q "C:\hostedtoolcache\windows\go" 2>nul
rmdir /s /q "C:\hostedtoolcache\windows\stack" 2>nul
rmdir /s /q "C:\hostedtoolcache\windows\Java_Temurin-Hotspot_jdk" 2>nul
rmdir /s /q "C:\Program Files\MySQL" 2>nul
rmdir /s /q "C:\Program Files\MongoDB" 2>nul
rmdir /s /q "C:\Strawberry" 2>nul
rmdir /s /q "C:\vcpkg" 2>nul
rmdir /s /q "C:\tools" 2>nul

python %FullScriptPath%prepare.py %*
if %errorlevel% neq 0 goto error

exit /b

:skip_job
echo [Retrogram CI] Skipping non-target matrix job in win.bat to save runner minutes.
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

