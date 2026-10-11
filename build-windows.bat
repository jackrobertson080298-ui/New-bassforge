@echo off
setlocal
where cmake >nul 2>nul || (echo Install CMake and add it to PATH & exit /b 1)
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 || exit /b 1
cmake --build build --config Release --parallel || exit /b 1
echo.
echo Built products are under build\BASSFORGE_artefacts\Release\
endlocal
