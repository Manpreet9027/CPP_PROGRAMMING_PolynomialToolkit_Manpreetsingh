@echo off
echo ========================================================
echo   Building Polynomial & Calculus Toolkit (C++17)
echo ========================================================
g++ -std=c++17 -Iinclude main.cpp src/Polynomial.cpp -o PolynomialToolkit.exe
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful! Launching Toolkit...
    echo.
    PolynomialToolkit.exe
) else (
    echo.
    echo [ERROR] Build failed. Please ensure MinGW g++ is installed and added to PATH.
    pause
)
