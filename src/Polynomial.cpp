#include "Polynomial.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <stdexcept>

// ==========================================
// Constructors
// ==========================================

Polynomial::Polynomial() {
    // Represents the zero polynomial (empty terms vector)
}

Polynomial::Polynomial(double constant) {
    if (std::abs(constant) > 1e-9) {
        terms.emplace_back(constant, 0);
    }
}

Polynomial::Polynomial(const std::vector<Term>& initialTerms) : terms(initialTerms) {
    simplify();
}

// ==========================================
// Dynamic Term Management
// ==========================================

void Polynomial::addTerm(double coeff, int exp) {
    if (std::abs(coeff) > 1e-9) {
        terms.emplace_back(coeff, exp);
    }
}

void Polynomial::simplify() {
    if (terms.empty()) return;

    // 1. Sort descending by exponent: O(n log n)
    std::sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
        return a.exp > b.exp;
    });

    // 2. Combine like terms with identical exponent: O(n)
    std::vector<Term> combined;
    for (const auto& t : terms) {
        if (std::abs(t.coeff) < 1e-9) continue;
        if (!combined.empty() && combined.back().exp == t.exp) {
            combined.back().coeff += t.coeff;
            if (std::abs(combined.back().coeff) < 1e-9) {
                combined.pop_back();
            }
        } else {
            combined.push_back(t);
        }
    }

    terms = std::move(combined);
}

int Polynomial::degree() const {
    if (terms.empty()) return 0;
    return terms.front().exp;
}

bool Polynomial::isZero() const {
    return terms.empty();
}

const std::vector<Term>& Polynomial::getTerms() const {
    return terms;
}

// ==========================================
// Operator Overloading
// ==========================================

Polynomial Polynomial::operator+(const Polynomial& other) const {
    Polynomial res;
    res.terms.reserve(terms.size() + other.terms.size());
    for (const auto& t : terms) res.terms.push_back(t);
    for (const auto& t : other.terms) res.terms.push_back(t);
    res.simplify();
    return res;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
    Polynomial res;
    res.terms.reserve(terms.size() + other.terms.size());
    for (const auto& t : terms) res.terms.push_back(t);
    for (const auto& t : other.terms) res.terms.emplace_back(-t.coeff, t.exp);
    res.simplify();
    return res;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
    if (isZero() || other.isZero()) {
        return Polynomial();
    }
    Polynomial res;
    res.terms.reserve(terms.size() * other.terms.size());
    // Outer product: O(n * m)
    for (const auto& t1 : terms) {
        for (const auto& t2 : other.terms) {
            res.terms.emplace_back(t1.coeff * t2.coeff, t1.exp + t2.exp);
        }
    }
    res.simplify();
    return res;
}

bool Polynomial::operator==(const Polynomial& other) const {
    Polynomial a = *this;
    Polynomial b = other;
    a.simplify();
    b.simplify();
    if (a.terms.size() != b.terms.size()) return false;
    for (size_t i = 0; i < a.terms.size(); ++i) {
        if (a.terms[i].exp != b.terms[i].exp) return false;
        if (std::abs(a.terms[i].coeff - b.terms[i].coeff) > 1e-7) return false;
    }
    return true;
}

bool Polynomial::operator!=(const Polynomial& other) const {
    return !(*this == other);
}

// ==========================================
// Algebraic Operations
// ==========================================

void Polynomial::divide(const Polynomial& divisor, Polynomial& quotient, Polynomial& remainder) const {
    if (divisor.isZero()) {
        throw std::runtime_error("Division by zero polynomial is undefined!");
    }

    quotient = Polynomial();
    remainder = *this;
    remainder.simplify();

    Polynomial div = divisor;
    div.simplify();

    while (!remainder.isZero() && remainder.degree() >= div.degree()) {
        double leadCoeff = remainder.terms[0].coeff / div.terms[0].coeff;
        int leadExp = remainder.terms[0].exp - div.terms[0].exp;

        Polynomial stepPoly;
        stepPoly.addTerm(leadCoeff, leadExp);

        quotient = quotient + stepPoly;
        remainder = remainder - (stepPoly * div);
        remainder.simplify();
    }
}

Polynomial Polynomial::compose(const Polynomial& g) const {
    if (terms.empty()) return Polynomial();

    // Polynomial composition P(g(x)) using Horner's method
    Polynomial result(terms[0].coeff);
    for (size_t i = 1; i < terms.size(); ++i) {
        int powerDiff = terms[i - 1].exp - terms[i].exp;
        for (int p = 0; p < powerDiff; ++p) {
            result = result * g;
        }
        result = result + Polynomial(terms[i].coeff);
    }
    for (int p = 0; p < terms.back().exp; ++p) {
        result = result * g;
    }
    return result;
}

Polynomial Polynomial::gcd(const Polynomial& other) const {
    Polynomial a = *this;
    Polynomial b = other;
    a.simplify();
    b.simplify();

    // Euclidean Algorithm for Polynomials
    while (!b.isZero()) {
        Polynomial q, r;
        a.divide(b, q, r);
        a = b;
        b = r;
    }

    // Normalize GCD to make leading coefficient 1.0 (monic)
    if (!a.isZero()) {
        double lead = a.terms[0].coeff;
        for (auto& t : a.terms) {
            t.coeff /= lead;
        }
        a.simplify();
    }
    return a;
}

void Polynomial::printFactors() const {
    Polynomial p = *this;
    p.simplify();

    if (p.isZero()) {
        std::cout << "0\n";
        return;
    }

    int deg = p.degree();
    if (deg == 0) {
        std::cout << p.terms[0].coeff << "\n";
        return;
    }

    if (deg == 1) {
        // ax + b = a(x + b/a)
        double a = p.terms[0].coeff;
        double b = (p.terms.size() > 1 && p.terms[1].exp == 0) ? p.terms[1].coeff : 0.0;
        double root = -b / a;
        if (std::abs(a - 1.0) > 1e-6) {
            std::cout << a << " * ";
        }
        std::cout << "(x " << (root >= 0 ? "- " : "+ ") << std::abs(root) << ")\n";
        std::cout << "Root: x = " << root << "\n";
        return;
    }

    if (deg == 2) {
        // ax^2 + bx + c
        double a = p.terms[0].coeff;
        double b = 0.0, c = 0.0;
        for (const auto& t : p.terms) {
            if (t.exp == 1) b = t.coeff;
            if (t.exp == 0) c = t.coeff;
        }
        double disc = b * b - 4 * a * c;
        if (disc >= 0) {
            double r1 = (-b + std::sqrt(disc)) / (2 * a);
            double r2 = (-b - std::sqrt(disc)) / (2 * a);
            if (std::abs(a - 1.0) > 1e-6) {
                std::cout << a << " * ";
            }
            std::cout << "(x " << (r1 >= 0 ? "- " : "+ ") << std::abs(r1) << ")"
                      << " * (x " << (r2 >= 0 ? "- " : "+ ") << std::abs(r2) << ")\n";
            std::cout << "Real Roots: x1 = " << r1 << ", x2 = " << r2 << "\n";
            return;
        } else {
            std::cout << p << " (Irreducible quadratic over Reals, Discriminant = " << disc << " < 0)\n";
            std::cout << "Complex Conjugate Roots: " << (-b / (2 * a)) << " +/- " << (std::sqrt(-disc) / (2 * a)) << "i\n";
            return;
        }
    }

    // General case (degree >= 3): Find real roots and factorize
    std::vector<double> roots;
    Polynomial current = p;

    // Scan for real roots in range [-50, 50]
    for (double x = -50.0; x <= 50.0; x += 0.25) {
        try {
            double r = current.findRoot(x, 60, 1e-6);
            if (std::abs(current.evaluate(r)) < 1e-4) {
                // Check if distinct
                bool exists = false;
                for (double existing : roots) {
                    if (std::abs(existing - r) < 1e-2) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    roots.push_back(r);
                }
            }
        } catch (...) {}
    }

    if (roots.empty()) {
        std::cout << "No simple rational/real roots found numerically.\n";
        std::cout << "Expression: " << p << "\n";
    } else {
        std::cout << "Identified Real Roots:\n";
        for (size_t i = 0; i < roots.size(); ++i) {
            std::cout << "  Root " << (i + 1) << ": x = " << roots[i]
                      << "  (Linear Factor: (x " << (roots[i] >= 0 ? "- " : "+ ") << std::abs(roots[i]) << "))\n";
        }
    }
}

// ==========================================
// Calculus Engine
// ==========================================

Polynomial Polynomial::differentiate() const {
    Polynomial deriv;
    for (const auto& t : terms) {
        if (t.exp > 0) {
            deriv.addTerm(t.coeff * t.exp, t.exp - 1);
        }
    }
    deriv.simplify();
    return deriv;
}

Polynomial Polynomial::nthDerivative(int order) const {
    if (order <= 0) return *this;
    Polynomial result = *this;
    for (int i = 0; i < order; ++i) {
        result = result.differentiate();
        if (result.isZero()) break;
    }
    return result;
}

Polynomial Polynomial::indefiniteIntegral(double c) const {
    Polynomial integral;
    for (const auto& t : terms) {
        integral.addTerm(t.coeff / (t.exp + 1), t.exp + 1);
    }
    if (std::abs(c) > 1e-9) {
        integral.addTerm(c, 0);
    }
    integral.simplify();
    return integral;
}

double Polynomial::definiteIntegral(double a, double b) const {
    Polynomial F = indefiniteIntegral(0.0);
    return F.evaluate(b) - F.evaluate(a);
}

// ==========================================
// Numerical Methods & Geometry
// ==========================================

// Horner's Method: O(n) linear evaluation
double Polynomial::evaluate(double x) const {
    if (terms.empty()) return 0.0;

    double result = terms[0].coeff;
    for (size_t i = 1; i < terms.size(); ++i) {
        int powerDiff = terms[i - 1].exp - terms[i].exp;
        for (int p = 0; p < powerDiff; ++p) {
            result *= x;
        }
        result += terms[i].coeff;
    }
    for (int p = 0; p < terms.back().exp; ++p) {
        result *= x;
    }
    return result;
}

void Polynomial::tangentLine(double x0, double& m, double& c) const {
    double y0 = evaluate(x0);
    Polynomial deriv = differentiate();
    m = deriv.evaluate(x0);
    c = y0 - m * x0;
}

void Polynomial::normalLine(double x0, double& m, double& c) const {
    double y0 = evaluate(x0);
    Polynomial deriv = differentiate();
    double mTangent = deriv.evaluate(x0);

    if (std::abs(mTangent) < 1e-9) {
        // Vertical normal line: x = x0
        m = 1e9;
        c = x0;
    } else {
        m = -1.0 / mTangent;
        c = y0 - m * x0;
    }
}

// Newton-Raphson Method
double Polynomial::findRoot(double guess, int maxIter, double tol) const {
    Polynomial pPrime = differentiate();
    double x = guess;

    for (int i = 0; i < maxIter; ++i) {
        double fx = evaluate(x);
        if (std::abs(fx) < tol) {
            return x;
        }

        double dfx = pPrime.evaluate(x);
        if (std::abs(dfx) < 1e-12) {
            // Nudge to escape flat slope / local extremum
            x += 0.01;
            continue;
        }

        double nextX = x - (fx / dfx);
        if (std::abs(nextX - x) < tol) {
            return nextX;
        }
        x = nextX;
    }

    return x;
}

void Polynomial::findCriticalPoints(double start, double end) const {
    Polynomial pPrime = differentiate();
    Polynomial pDoublePrime = pPrime.differentiate();

    if (pPrime.isZero()) {
        std::cout << "Polynomial is constant; all points have zero derivative.\n";
        return;
    }

    std::vector<double> criticalPoints;
    double step = (end - start) / 400.0;

    for (double x = start; x <= end; x += step) {
        double y1 = pPrime.evaluate(x);
        double y2 = pPrime.evaluate(x + step);

        if (y1 * y2 <= 0.0 || std::abs(y1) < 1e-4) {
            double root = pPrime.findRoot(x, 50, 1e-6);
            if (root >= start - 1e-4 && root <= end + 1e-4) {
                bool alreadyFound = false;
                for (double cp : criticalPoints) {
                    if (std::abs(cp - root) < 1e-3) {
                        alreadyFound = true;
                        break;
                    }
                }
                if (!alreadyFound) {
                    criticalPoints.push_back(root);
                }
            }
        }
    }

    if (criticalPoints.empty()) {
        std::cout << "No critical points found in interval [" << start << ", " << end << "].\n";
        return;
    }

    std::sort(criticalPoints.begin(), criticalPoints.end());
    std::cout << std::left << std::setw(12) << "x"
              << std::setw(15) << "f(x)"
              << std::setw(15) << "f''(x)"
              << "Classification\n";
    std::cout << "--------------------------------------------------------\n";

    for (double cp : criticalPoints) {
        double fx = evaluate(cp);
        double fxx = pDoublePrime.evaluate(cp);
        std::string classification;

        if (fxx > 1e-5) {
            classification = "Local Minimum";
        } else if (fxx < -1e-5) {
            classification = "Local Maximum";
        } else {
            classification = "Point of Inflection";
        }

        std::cout << std::fixed << std::setprecision(4)
                  << std::left << std::setw(12) << cp
                  << std::setw(15) << fx
                  << std::setw(15) << fxx
                  << classification << "\n";
    }
}

// ==========================================
// Parsing Logic (Recursive Descent Parser)
// ==========================================

namespace {
    struct Tokenizer {
        std::string src;
        size_t pos = 0;

        Tokenizer(const std::string& str) : src(str), pos(0) {}

        void skipWhitespace() {
            while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos]))) {
                ++pos;
            }
        }

        char peek() {
            skipWhitespace();
            if (pos >= src.size()) return '\0';
            return src[pos];
        }

        char get() {
            skipWhitespace();
            if (pos >= src.size()) return '\0';
            return src[pos++];
        }
    };

    // Forward declarations for recursive descent grammar
    Polynomial parseExpression(Tokenizer& tok);
    Polynomial parseTerm(Tokenizer& tok);
    Polynomial parseFactor(Tokenizer& tok);
    Polynomial parseBase(Tokenizer& tok);

    Polynomial parseExpression(Tokenizer& tok) {
        Polynomial result = parseTerm(tok);
        while (true) {
            char op = tok.peek();
            if (op == '+') {
                tok.get();
                result = result + parseTerm(tok);
            } else if (op == '-') {
                tok.get();
                result = result - parseTerm(tok);
            } else {
                break;
            }
        }
        return result;
    }

    Polynomial parseTerm(Tokenizer& tok) {
        Polynomial result = parseFactor(tok);
        while (true) {
            char op = tok.peek();
            if (op == '*') {
                tok.get();
                result = result * parseFactor(tok);
            } else if (op == '(' || std::isalpha(static_cast<unsigned char>(op))) {
                // Implicit multiplication: e.g. 2(x+1) or (x+1)(x-1) or 3x
                result = result * parseFactor(tok);
            } else {
                break;
            }
        }
        return result;
    }

    Polynomial parseFactor(Tokenizer& tok) {
        Polynomial base = parseBase(tok);
        if (tok.peek() == '^') {
            tok.get();
            tok.skipWhitespace();
            int exponent = 0;
            bool negExp = false;
            if (tok.peek() == '+') tok.get();
            else if (tok.peek() == '-') { negExp = true; tok.get(); }

            while (std::isdigit(static_cast<unsigned char>(tok.peek()))) {
                exponent = exponent * 10 + (tok.get() - '0');
            }
            if (negExp) {
                throw std::runtime_error("Negative exponents are not supported in polynomial algebra!");
            }
            if (exponent == 0) {
                return Polynomial(1.0);
            }
            Polynomial powerResult(1.0);
            for (int i = 0; i < exponent; ++i) {
                powerResult = powerResult * base;
            }
            return powerResult;
        }
        return base;
    }

    Polynomial parseBase(Tokenizer& tok) {
        tok.skipWhitespace();
        char c = tok.peek();

        // Unary minus or plus
        if (c == '-') {
            tok.get();
            return Polynomial(-1.0) * parseFactor(tok);
        }
        if (c == '+') {
            tok.get();
            return parseFactor(tok);
        }

        // Sub-expression in parentheses
        if (c == '(') {
            tok.get(); // consume '('
            Polynomial sub = parseExpression(tok);
            if (tok.get() != ')') {
                throw std::runtime_error("Mismatched parentheses in polynomial expression!");
            }
            return sub;
        }

        // Variable 'x' or 'X'
        if (c == 'x' || c == 'X') {
            tok.get();
            Polynomial p;
            p.addTerm(1.0, 1);
            return p;
        }

        // Numeric literal
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            std::string numStr;
            while (std::isdigit(static_cast<unsigned char>(tok.peek())) || tok.peek() == '.') {
                numStr += tok.get();
            }
            double val = std::stod(numStr);
            return Polynomial(val);
        }

        throw std::runtime_error(std::string("Unexpected token in expression: '") + c + "'");
    }
}

Polynomial Polynomial::parse(const std::string& expr) {
    if (expr.empty()) return Polynomial();
    Tokenizer tok(expr);
    Polynomial p = parseExpression(tok);
    p.simplify();
    return p;
}

// ==========================================
// Formatting & Stream I/O
// ==========================================

std::string Polynomial::toString() const {
    if (terms.empty()) {
        return "0";
    }

    std::ostringstream oss;
    bool first = true;

    for (const auto& t : terms) {
        double c = t.coeff;
        int e = t.exp;

        if (first) {
            if (c < 0) oss << "-";
        } else {
            if (c < 0) oss << " - ";
            else oss << " + ";
        }

        double absC = std::abs(c);

        if (e == 0) {
            oss << absC;
        } else {
            if (std::abs(absC - 1.0) > 1e-9) {
                oss << absC;
            }
            oss << "x";
            if (e > 1) {
                oss << "^" << e;
            }
        }

        first = false;
    }

    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
    os << p.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& p) {
    p = Polynomial();
    int deg = 0;
    if (!(is >> deg)) return is;
    if (deg < 0) return is;

    // Reads coefficients c0, c1, ... up to cDeg
    for (int i = 0; i <= deg; ++i) {
        double coeff = 0.0;
        if (is >> coeff) {
            p.addTerm(coeff, i);
        }
    }
    p.simplify();
    return is;
}
