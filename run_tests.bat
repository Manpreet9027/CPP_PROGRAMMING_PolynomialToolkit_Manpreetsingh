@echo off
echo ========================================================
echo   Compiling and Running Automated Verification Tests
echo ========================================================
g++ -std=c++17 -Iinclude tests/test_polynomial.cpp src/Polynomial.cpp -o runTests.exe
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Running unit tests...
    echo.
    runTests.exe
    pause
) else (
    echo.
    echo [ERROR] Test build failed. Please check compiler output above.
    pause
)
