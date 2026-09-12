#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

/**
 * @brief Represents a single term in a polynomial: coeff * x^exp
 */
struct Term {
    double coeff; // Coefficient
    int exp;      // Exponent / power of x

    Term(double c = 0.0, int e = 0) : coeff(c), exp(e) {}

    bool operator==(const Term& other) const {
        return (exp == other.exp) && (std::fabs(coeff - other.coeff) < 1e-9);
    }
};

/**
 * @brief Polynomial class implementing symbolic algebra, calculus, and numerical methods.
 */
class Polynomial {
private:
    std::vector<Term> terms; // Dynamic list of terms, kept sorted by descending exponent

public:
    // Constructors
    Polynomial();
    explicit Polynomial(double constant);
    Polynomial(const std::vector<Term>& initialTerms);

    // Dynamic term management
    void addTerm(double coeff, int exp);
    void simplify();
    int degree() const;
    bool isZero() const;
    const std::vector<Term>& getTerms() const;

    // Operator Overloading
    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;
    bool operator==(const Polynomial& other) const;
    bool operator!=(const Polynomial& other) const;

    // Algebraic Operations
    void divide(const Polynomial& divisor, Polynomial& quotient, Polynomial& remainder) const;
    Polynomial compose(const Polynomial& g) const;
    Polynomial gcd(const Polynomial& other) const;
    void printFactors() const;

    // Calculus Engine
    Polynomial differentiate() const;
    Polynomial nthDerivative(int order) const;
    Polynomial indefiniteIntegral(double c = 0.0) const;
    double definiteIntegral(double a, double b) const;

    // Numerical Methods & Geometry
    double evaluate(double x) const; // Horner's Method (O(n))
    void tangentLine(double x0, double& m, double& c) const;
    void normalLine(double x0, double& m, double& c) const;
    double findRoot(double guess, int maxIter = 100, double tol = 1e-7) const; // Newton-Raphson
    void findCriticalPoints(double start, double end) const;

    // Parsing and Formatting
    static Polynomial parse(const std::string& expr);
    std::string toString() const;

    // Stream I/O Overloads
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);
    friend std::istream& operator>>(std::istream& is, Polynomial& p);
};

#endif // POLYNOMIAL_H
