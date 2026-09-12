#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include "../include/Polynomial.h"

// Simple test framework macro for clean, standalone verification
static int totalTests = 0;
static int passedTests = 0;

void reportTest(const std::string& testName, const std::string& input, const std::string& expected, const std::string& observed, bool pass) {
    ++totalTests;
    if (pass) ++passedTests;
    std::cout << std::left << std::setw(26) << testName
              << std::setw(30) << input
              << std::setw(20) << expected
              << std::setw(20) << observed
              << (pass ? " [PASSED]" : " [FAILED]") << "\n";
}

int main() {
    std::cout << "========================================================================================\n";
    std::cout << "          POLYNOMIAL CALCULUS TOOLKIT - AUTOMATED VERIFICATION SUITE                   \n";
    std::cout << "          DSA & Object-Oriented C++ Programming                                         \n";
    std::cout << "========================================================================================\n";
    std::cout << std::left << std::setw(26) << "Test Case"
              << std::setw(30) << "Input / Operation"
              << std::setw(20) << "Expected Result"
              << std::setw(20) << "Observed Result"
              << "Status\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    // -------------------------------------------------------------
    // Test 1: Term Simplification (Report Table 6.1)
    // -------------------------------------------------------------
    {
        Polynomial p;
        p.addTerm(3.0, 2);
        p.addTerm(2.0, 2);
        p.addTerm(-5.0, 0);
        p.simplify();
        std::string expected = "5x^2 - 5";
        std::string observed = p.toString();
        reportTest("Term Simplification", "3x^2 + 2x^2 - 5", expected, observed, observed == expected);
    }

    // -------------------------------------------------------------
    // Test 2: Polynomial Addition (Report Table 6.1)
    // -------------------------------------------------------------
    {
        Polynomial p1 = Polynomial::parse("x^2 + 2x");
        Polynomial p2 = Polynomial::parse("2x^2 - x");
        Polynomial sum = p1 + p2;
        std::string expected = "3x^2 + x";
        std::string observed = sum.toString();
        reportTest("Polynomial Addition", "(x^2 + 2x) + (2x^2 - x)", expected, observed, observed == expected);
    }

    // -------------------------------------------------------------
    // Test 3: Multiplication (Report Table 6.1)
    // -------------------------------------------------------------
    {
        Polynomial p1 = Polynomial::parse("x + 1");
        Polynomial p2 = Polynomial::parse("x - 1");
        Polynomial prod = p1 * p2;
        std::string expected = "x^2 - 1";
        std::string observed = prod.toString();
        reportTest("Multiplication", "(x + 1) * (x - 1)", expected, observed, observed == expected);
    }

    // -------------------------------------------------------------
    // Test 4: Differentiation (Report Table 6.1)
    // -------------------------------------------------------------
    {
        Polynomial p = Polynomial::parse("4x^3 - 2x + 7");
        Polynomial diff = p.differentiate();
        std::string expected = "12x^2 - 2";
        std::string observed = diff.toString();
        reportTest("Differentiation", "d/dx(4x^3 - 2x + 7)", expected, observed, observed == expected);
    }

    // -------------------------------------------------------------
    // Test 5: Indefinite Integration (Report Table 6.1)
    // -------------------------------------------------------------
    {
        Polynomial p = Polynomial::parse("3x^2 + 2x");
        Polynomial integ = p.indefiniteIntegral(0.0);
        std::string expected = "x^3 + x^2";
        std::string observed = integ.toString();
        reportTest("Indefinite Integration", "int(3x^2 + 2x) dx", expected + " + C", observed + " + C", observed == expected);
    }

    // -------------------------------------------------------------
    // Test 6: Horner Evaluation (Report Table 6.1)
    // -------------------------------------------------------------
    {
        Polynomial p = Polynomial::parse("2x^2 + 3x + 1");
        double val = p.evaluate(2.0);
        std::string expected = "15.0000";
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(4) << val;
        std::string observed = oss.str();
        reportTest("Horner Evaluation", "P(2) for 2x^2 + 3x + 1", "15", observed, std::fabs(val - 15.0) < 1e-6);
    }

    // -------------------------------------------------------------
    // Core Extended Verification Tests
    // -------------------------------------------------------------

    // Test 7: Polynomial Long Division
    {
        Polynomial p1 = Polynomial::parse("x^2 - 1");
        Polynomial p2 = Polynomial::parse("x - 1");
        Polynomial q, r;
        p1.divide(p2, q, r);
        std::string expected = "x + 1";
        std::string observed = q.toString();
        reportTest("Polynomial Division", "(x^2 - 1) / (x - 1)", expected, observed, observed == expected && r.isZero());
    }

    // Test 8: Nth Derivative (Order = 2)
    {
        Polynomial p = Polynomial::parse("4x^3 - 2x + 7");
        Polynomial d2 = p.nthDerivative(2);
        std::string expected = "24x";
        std::string observed = d2.toString();
        reportTest("2nd-Order Derivative", "d^2/dx^2(4x^3 - 2x + 7)", expected, observed, observed == expected);
    }

    // Test 9: Definite Integral
    {
        Polynomial p = Polynomial::parse("3x^2 + 2x");
        double area = p.definiteIntegral(0.0, 2.0); // (2^3 + 2^2) - 0 = 8 + 4 = 12
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << area;
        std::string observed = oss.str();
        reportTest("Definite Integral", "int[0 to 2] (3x^2+2x)dx", "12.00", observed, std::fabs(area - 12.0) < 1e-6);
    }

    // Test 10: Newton-Raphson Root Finding
    {
        Polynomial p = Polynomial::parse("x^2 - 4");
        double root = p.findRoot(1.5);
        bool pass = std::fabs(root - 2.0) < 1e-4;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(4) << root;
        reportTest("Newton-Raphson Root", "Root of x^2 - 4 near 1.5", "2.0000", oss.str(), pass);
    }

    // Test 11: Polynomial Composition
    {
        Polynomial f = Polynomial::parse("x^2");
        Polynomial g = Polynomial::parse("x + 1");
        Polynomial fog = f.compose(g); // (x+1)^2 = x^2 + 2x + 1
        std::string expected = "x^2 + 2x + 1";
        std::string observed = fog.toString();
        reportTest("Polynomial Composition", "(x + 1)^2 via compose", expected, observed, observed == expected);
    }

    // Test 12: Polynomial GCD
    {
        Polynomial a = Polynomial::parse("x^2 - 1");
        Polynomial b = Polynomial::parse("x - 1");
        Polynomial g = a.gcd(b);
        std::string expected = "x - 1";
        std::string observed = g.toString();
        reportTest("Polynomial GCD", "GCD(x^2 - 1, x - 1)", expected, observed, observed == expected);
    }

    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << "SUMMARY: " << passedTests << " / " << totalTests << " tests passed ("
              << (passedTests * 100 / totalTests) << "% pass rate)\n";
    std::cout << "========================================================================================\n";

    return (passedTests == totalTests) ? 0 : 1;
}
