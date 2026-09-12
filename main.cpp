#include <iostream>
#include <limits>
#include <cmath>
#include <string>
#include "include/Polynomial.h"

// Helper function to clear invalid console input
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Template helper for safe input reading
template <typename T>
T getValidInput(const std::string& prompt) {
    T val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            return val;
        }
        std::cout << "Invalid input. Please enter a valid number.\n";
        clearInputBuffer();
    }
}

// Prompt user to enter a polynomial via string expression or raw coefficients
Polynomial inputPolynomialPrompt(const std::string& label) {
    std::cout << "\nChoose input method for " << label << ":\n";
    std::cout << "  1. String Expression (e.g. (x+1)^2 or 3x^2 - 5x + 1 or 4x^3 - 2x + 7)\n";
    std::cout << "  2. Raw Degree & Coefficients (c0 to cN)\n";
    int mode = getValidInput<int>("Select option (1-2): ");

    if (mode == 1) {
        while (true) {
            std::cout << "Enter expression: ";
            std::string expr;
            std::cin >> std::ws;
            std::getline(std::cin, expr);
            try {
                return Polynomial::parse(expr);
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\nPlease try again.\n";
            }
        }
    } else {
        Polynomial p;
        std::cout << "Enter degree followed by coefficients (c0 to cN):\n";
        std::cin >> p;
        return p;
    }
}

std::string getOrdinalSuffix(int n) {
    if (n % 100 >= 11 && n % 100 <= 13) return "th";
    switch (n % 10) {
        case 1:  return "st";
        case 2:  return "nd";
        case 3:  return "rd";
        default: return "th";
    }
}

void askToSaveResult(Polynomial& p1, const Polynomial& result) {
    char choice = getValidInput<char>("Save result as new primary P1? (Y/N): ");
    if (choice == 'y' || choice == 'Y') {
        p1 = result;
        std::cout << "Updated primary P1(x) = " << p1 << "\n";
    }
}

int main() {
    Polynomial p1, p2;
    int choice = 0;

    std::cout << "=======================================================\n";
    std::cout << "          POLYNOMIAL CALCULUS TOOLKIT (C++)            \n";
    std::cout << "       DSA & Object-Oriented C++ Programming           \n";
    std::cout << "=======================================================\n";

    p1 = inputPolynomialPrompt("Primary Polynomial P1");

    while (true) {
        std::cout << "\n-------------------------------------------------------\n";
        std::cout << "Current Polynomial: P1(x) = " << p1 << "\n";
        std::cout << "Degree: " << p1.degree() << "\n";
        std::cout << "-------------------------------------------------------\n";
        std::cout << " 1. Add another polynomial (P1 + P2)\n";
        std::cout << " 2. Subtract another polynomial (P1 - P2)\n";
        std::cout << " 3. Multiply by another polynomial (P1 * P2)\n";
        std::cout << " 4. Divide by another polynomial (P1 / P2)\n";
        std::cout << " 5. Derivative (1st, 2nd, Nth Order)\n";
        std::cout << " 6. Integration (Indefinite & Definite)\n";
        std::cout << " 7. Evaluate at x (Horner's Method)\n";
        std::cout << " 8. Find Root (Newton-Raphson Method)\n";
        std::cout << " 9. Re-enter Primary Polynomial P1\n";
        std::cout << "10. Exit\n";

        choice = getValidInput<int>("Select an option (1-10): ");

        if (choice == 10) {
            std::cout << "\nExiting Polynomial Calculus Toolkit. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                p2 = inputPolynomialPrompt("Polynomial P2 to add");
                Polynomial res = p1 + p2;
                std::cout << "\nResult of Addition:\n";
                std::cout << "(" << p1 << ") + (" << p2 << ") = " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 2: {
                p2 = inputPolynomialPrompt("Polynomial P2 to subtract");
                Polynomial res = p1 - p2;
                std::cout << "\nResult of Subtraction:\n";
                std::cout << "(" << p1 << ") - (" << p2 << ") = " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 3: {
                p2 = inputPolynomialPrompt("Polynomial P2 to multiply");
                Polynomial res = p1 * p2;
                std::cout << "\nResult of Multiplication:\n";
                std::cout << "(" << p1 << ") * (" << p2 << ") = " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 4: {
                p2 = inputPolynomialPrompt("Divisor Polynomial P2");
                if (p2.isZero()) {
                    std::cout << "\nError: Division by zero polynomial is undefined!\n";
                } else {
                    Polynomial quotient, remainder;
                    p1.divide(p2, quotient, remainder);
                    std::cout << "\nResult of Polynomial Division:\n";
                    std::cout << "Quotient:  Q(x) = " << quotient << "\n";
                    std::cout << "Remainder: R(x) = " << remainder << "\n";
                    askToSaveResult(p1, quotient);
                }
                break;
            }
            case 5: {
                int order;
                while (true) {
                    order = getValidInput<int>("Enter derivative order (1, 2, 3...): ");
                    if (order > 0) break;
                    std::cout << "Derivative order must be a positive integer.\n";
                }
                Polynomial res = p1.nthDerivative(order);
                std::cout << "\n" << order << getOrdinalSuffix(order) << " Derivative of [" << p1 << "]:\n";
                std::cout << "P1^(" << order << ")(x) = " << res << "\n";
                askToSaveResult(p1, res);
                break;
            }
            case 6: {
                double c = getValidInput<double>("Enter constant of integration C (e.g. 0): ");
                Polynomial indef = p1.indefiniteIntegral(c);
                std::cout << "\nIndefinite Integral:\n";
                std::cout << "∫ (" << p1 << ") dx = " << indef;
                if (std::fabs(c) < 1e-9) std::cout << " + C";
                std::cout << "\n\n";

                double a = getValidInput<double>("Enter lower bound (a) for Definite Integral: ");
                double b = getValidInput<double>("Enter upper bound (b) for Definite Integral: ");
                double area = p1.definiteIntegral(a, b);
                std::cout << "Definite Integral from a = " << a << " to b = " << b << ":\n";
                std::cout << "∫[" << a << " to " << b << "] (" << p1 << ") dx = " << area << "\n";
                break;
            }
            case 7: {
                double x = getValidInput<double>("Enter value x to evaluate P1(x): ");
                double val = p1.evaluate(x);
                std::cout << "\nEvaluation using Horner's Method (O(n)):\n";
                std::cout << "P1(" << x << ") = " << val << "\n";
                break;
            }
            case 8: {
                double guess = getValidInput<double>("Enter initial guess for Newton-Raphson root finding: ");
                double root = p1.findRoot(guess);
                std::cout << "\nNewton-Raphson Numerical Root Finding:\n";
                std::cout << "Approximated Root: x = " << root << "\n";
                std::cout << "Verification Check: P1(" << root << ") = " << p1.evaluate(root) << "\n";
                break;
            }
            case 9: {
                p1 = inputPolynomialPrompt("New Primary Polynomial P1");
                break;
            }
            default:
                std::cout << "Invalid option. Please enter a number between 1 and 10.\n";
                break;
        }
    }

    return 0;
}
