# Polynomial Calculus Toolkit

> **A C++ Core and Interactive Web-Based System for Polynomial Mathematics & Calculus Operations**  
> **Course:** Data Structures and Algorithms (DSA) / C++ Programming  
> **Candidate:** Manpreet Singh (Registration No. `12413521`)  
> **Programme:** B.Tech Computer Science and Engineering  
> **GitHub Repository:** [CPP_PROGRAMMING_PolynomialToolkit_Manpreetsingh](https://github.com/Manpreet9027/CPP_PROGRAMMING_PolynomialToolkit_Manpreetsingh)

---

## 1. Project Abstract & Overview

The **Polynomial Calculus Toolkit** is a mathematical application designed to represent, manipulate, and execute calculus operations on single-variable polynomials $P(x)$. Rather than relying on external third-party math packages, all core algebraic algorithms, term normalization routines, symbolic differentiation, integration, and numerical root-finding algorithms are constructed **from first principles** in Standard C++17.

The toolkit features:
1. **High-Performance C++ Core Engine**: Implements Object-Oriented Programming (OOP) with dynamic memory storage (`std::vector<Term>`), operator overloading, and $O(n)$ Horner's rule evaluation.
2. **10-Option Interactive Command-Line Interface (CLI)**: Enables real-time arithmetic (addition, subtraction, multiplication, division), $N^{\text{th}}$-order derivatives, definite/indefinite integration, Horner evaluation, and Newton-Raphson roots.
3. **Automated Verification Test Suite**: Automated unit tests validating all 6 core operations from the Project Report and Presentation with a 100% pass rate.
4. **Interactive Web Dashboard**: A visual front-end built with HTML5, CSS3, vanilla JavaScript, and Chart.js to interactively plot $f(x)$ alongside its derivative $f'(x)$.

---

## 2. Technology Stack

| Layer | Component | Specification |
| :--- | :--- | :--- |
| **Core Engine** | Language | C++ (Standard C++17) |
| | Paradigm | Object-Oriented Programming (OOP) |
| | Primary Data Structure | Dynamic Term Vector (`std::vector<Term>`) |
| **Build System** | Compilers / Tools | MinGW GCC (`g++`), CMake (v3.14+), Make |
| **Testing** | Verification Framework | Custom Automated Unit Test Suite (`tests/test_polynomial.cpp`) |
| **Web Dashboard** | Front End | HTML5, CSS3, Vanilla JavaScript (ES6), Chart.js |

---

## 3. System Architecture & OOP Design

The core logic is partitioned cleanly across header interfaces, implementation files, and driver applications:

```
cppmanpreet/
├── include/
│   └── Polynomial.h          # Class declaration, Term struct & API prototypes
├── src/
│   └── Polynomial.cpp        # Full algorithm implementations & parser
├── tests/
│   └── test_polynomial.cpp   # Automated unit test suite (13 test cases)
├── web/                      # Standalone web dashboard directory
│   ├── index.html
│   ├── style.css
│   └── app.js
├── main.cpp                  # 16-option interactive terminal menu
├── CMakeLists.txt            # Modern CMake build configuration
├── Makefile                  # Unix/MinGW makefile
├── build_and_run.bat         # 1-click Windows compile & run script
├── run_tests.bat             # 1-click Windows test runner
├── index.html                # Root web dashboard launcher
├── style.css                 # Dark theme responsive styling
├── app.js                    # Web calculus engine & Chart.js driver
├── README.md                 # Project documentation
└── VIVA_QUESTIONS_AND_ANSWERS.md  # Comprehensive Viva preparation handbook
```

### Key OOP Principles Demonstrated:
- **Encapsulation**: Polynomial terms (`std::vector<Term> terms`) are kept strictly `private`. Users interact exclusively via validated public member methods.
- **Abstraction**: High-level callers invoke `p.differentiate()` or `p.evaluate(x)` without needing to manage term memory, sorting, or power indexing.
- **Operator Overloading**: Overloaded algebraic operators (`+`, `-`, `*`, `==`, `!=`) and stream operators (`<<`, `>>`) allow natural mathematical expression handling.
- **Modularity**: Separation of the reusable engine (`Polynomial.h`, `Polynomial.cpp`), driver application (`main.cpp`), and verification tests (`test_polynomial.cpp`).

---

## 4. Algorithms & Complexity Analysis

| Operation | Technique Used | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Term Sorting** | Descending comparison sort | $O(n \log n)$ | $O(1)$ auxiliary |
| **Simplification Pass** | Linear like-term merge | $O(n)$ | $O(n)$ |
| **Addition / Subtraction** | Term concatenation + merge | $O(n + m)$ | $O(n + m)$ |
| **Multiplication** | Outer product + simplification | $O(n \cdot m)$ | $O(n \cdot m)$ |
| **Evaluation** | **Horner's Method** | **$O(n)$** | **$O(1)$** |
| **Differentiation** | Term-by-term Power Rule | $O(n)$ | $O(n)$ |
| **Integration** | Reverse Power Rule + constant $C$ | $O(n)$ | $O(n)$ |
| **Root Finding** | Newton-Raphson Iteration | $O(k \cdot n)$ | $O(1)$ |
| **Polynomial Division** | Long Division Algorithm | $O(\text{deg}(A) \cdot \text{deg}(B))$ | $O(\text{deg}(A))$ |

> **Why Horner's Method?**  
> Naive polynomial evaluation calculates each term $a_k \cdot x^k$ independently, taking $O(n^2)$ arithmetic operations. Horner's Method expresses $P(x)$ as:  
> $$P(x) = ((a_n \cdot x + a_{n-1})x + \dots)x + a_0$$  
> This performs exactly $n$ multiplications and $n$ additions, reducing the time complexity to optimal **$O(n)$**.

---

## 5. Verification & Unit Testing

The project includes an automated test harness covering all validation benchmarks specified in Section 6 of the Project Report:

| Test Case | Input Expression | Expected Result | Observed Result | Status |
| :--- | :--- | :--- | :--- | :--- |
| **Term Simplification** | `3x^2 + 2x^2 - 5` | `5x^2 - 5` | `5x^2 - 5` | **PASSED** |
| **Polynomial Addition** | `(x^2 + 2x) + (2x^2 - x)` | `3x^2 + x` | `3x^2 + x` | **PASSED** |
| **Multiplication** | `(x + 1) * (x - 1)` | `x^2 - 1` | `x^2 - 1` | **PASSED** |
| **Differentiation** | $\frac{d}{dx}(4x^3 - 2x + 7)$ | `12x^2 - 2` | `12x^2 - 2` | **PASSED** |
| **Indefinite Integration** | $\int (3x^2 + 2x) dx$ | `x^3 + x^2 + C` | `x^3 + x^2 + C` | **PASSED** |
| **Horner Evaluation** | $P(2)$ for $2x^2 + 3x + 1$ | `15` | `15.0000` | **PASSED** |
| **Polynomial Division** | $(x^2 - 1) / (x - 1)$ | `x + 1`, Rem: `0` | `x + 1`, Rem: `0` | **PASSED** |
| **$2^{\text{nd}}$-Order Derivative** | $\frac{d^2}{dx^2}(4x^3 - 2x + 7)$ | `24x` | `24x` | **PASSED** |
| **Definite Integration** | $\int_0^2 (3x^2 + 2x) dx$ | `12.00` | `12.00` | **PASSED** |
| **Newton-Raphson Root** | $x^2 - 4 = 0$ near $x=1.5$ | `2.0000` | `2.0000` | **PASSED** |
| **Polynomial Composition** | $(x + 1)^2$ via compose | `x^2 + 2x + 1` | `x^2 + 2x + 1` | **PASSED** |
| **Polynomial GCD** | $\gcd(x^2 - 1, x - 1)$ | `x - 1` | `x - 1` | **PASSED** |

**Pass Rate: 12 / 12 (100%)**

---

## 6. How to Build & Run

### Method A: One-Click Windows Batch Scripts (Recommended)
1. **Run the CLI Toolkit**: Double-click `build_and_run.bat` or run:
   ```cmd
   build_and_run.bat
   ```
2. **Run the Automated Tests**: Double-click `run_tests.bat` or run:
   ```cmd
   run_tests.bat
   ```

### Method B: Direct MinGW GCC Compilation
```cmd
# Compile the main CLI application:
g++ -std=c++17 -Iinclude main.cpp src/Polynomial.cpp -o PolynomialToolkit.exe
.\PolynomialToolkit.exe

# Compile and run the test suite:
g++ -std=c++17 -Iinclude tests/test_polynomial.cpp src/Polynomial.cpp -o runTests.exe
.\runTests.exe
```

### Method C: CMake Build
```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest --output-on-failure
./PolynomialToolkit
```

---

## 7. Interactive Web Dashboard

To launch the web visualizer:
1. Open `index.html` (or `web/index.html`) in any modern web browser (Chrome, Edge, Firefox).
2. Enter any expression (e.g. `(x+1)^2`, `4x^3 - 2x + 7`, or `3x^2 + 2x`) or click any test preset button.
3. The dashboard plots $f(x)$ and its derivative $f'(x)$ interactively and displays instantaneous calculations for derivatives, integrals, tangent lines, and normal lines.
