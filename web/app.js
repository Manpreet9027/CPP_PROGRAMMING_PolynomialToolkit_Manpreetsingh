let chartInstance = null;

// ==========================================
// Polynomial Data Structure & Algorithms (JS)
// ==========================================

class Term {
    constructor(coeff = 0, exp = 0) {
        this.coeff = Number(coeff);
        this.exp = parseInt(exp, 10);
    }
}

// Parses string expressions like (x+1)^2 or 3x^2 - 5x + 1 or 4x^3 - 2x + 7
function parsePolynomial(expr) {
    if (!expr || !expr.trim()) return [];

    // Expand squared binomials: (x ± a)^2 -> x^2 ± 2ax + a^2
    let s = expr.replace(/\((x)\s*([\+\-])\s*(\d+(\.\d+)?)\)\^2/g, (m, varX, sign, val) => {
        let a = parseFloat(val) * (sign === '-' ? -1 : 1);
        let doubleA = 2 * a;
        let aSq = a * a;
        return `x^2 ${doubleA >= 0 ? '+' : ''}${doubleA}x + ${aSq}`;
    });

    s = s.replace(/[\(\)]/g, '').replace(/\s+/g, '');

    // Tokenize terms matching ± ax^b, ± ax, ± a, ± x^b, ± x
    const regex = /([+-]?[^+-]+)/g;
    let matches = s.match(regex) || [];
    let terms = [];

    for (let raw of matches) {
        if (!raw) continue;
        let coeff = 1;
        let exp = 0;

        if (raw.includes('x') || raw.includes('X')) {
            let parts = raw.split(/[xX]/);
            if (parts[0] === '' || parts[0] === '+') {
                coeff = 1;
            } else if (parts[0] === '-') {
                coeff = -1;
            } else {
                coeff = parseFloat(parts[0]);
            }

            if (parts[1] && parts[1].startsWith('^')) {
                exp = parseInt(parts[1].substring(1), 10);
            } else {
                exp = 1;
            }
        } else {
            coeff = parseFloat(raw);
            exp = 0;
        }

        if (!isNaN(coeff) && Math.abs(coeff) > 1e-9) {
            terms.push(new Term(coeff, exp));
        }
    }

    return simplify(terms);
}

// Canonical ordering O(n log n) + Like-term combination pass O(n)
function simplify(terms) {
    if (!terms || terms.length === 0) return [];

    // 1. Sort descending by exponent
    terms.sort((a, b) => b.exp - a.exp);

    // 2. Merge terms with matching exponent
    let combined = [];
    for (let t of terms) {
        if (Math.abs(t.coeff) < 1e-9) continue;
        if (combined.length > 0 && combined[combined.length - 1].exp === t.exp) {
            combined[combined.length - 1].coeff += t.coeff;
            if (Math.abs(combined[combined.length - 1].coeff) < 1e-9) {
                combined.pop();
            }
        } else {
            combined.push(new Term(t.coeff, t.exp));
        }
    }
    return combined;
}

// Converts term list to standard readable string representation
function formatPolynomial(terms) {
    if (!terms || terms.length === 0) return "0";

    let res = "";
    for (let i = 0; i < terms.length; ++i) {
        let c = terms[i].coeff;
        let e = terms[i].exp;
        let absC = Math.abs(c);
        let roundC = Math.round(absC * 1000) / 1000;

        if (i === 0) {
            if (c < 0) res += "-";
        } else {
            res += (c < 0 ? " - " : " + ");
        }

        if (e === 0) {
            res += roundC;
        } else {
            if (Math.abs(roundC - 1.0) > 1e-9) {
                res += roundC;
            }
            res += "x";
            if (e > 1) {
                res += "^" + e;
            }
        }
    }
    return res;
}

// Horner's Method: Linear O(n) polynomial evaluation
function evaluatePoly(terms, x) {
    if (!terms || terms.length === 0) return 0;

    let res = terms[0].coeff;
    for (let i = 1; i < terms.length; ++i) {
        let diff = terms[i - 1].exp - terms[i].exp;
        for (let p = 0; p < diff; ++p) {
            res *= x;
        }
        res += terms[i].coeff;
    }
    for (let p = 0; p < terms[terms.length - 1].exp; ++p) {
        res *= x;
    }
    return res;
}

// Differentiation: Term-by-term power rule O(n)
function differentiate(terms) {
    let deriv = [];
    for (let t of terms) {
        if (t.exp > 0) {
            deriv.push(new Term(t.coeff * t.exp, t.exp - 1));
        }
    }
    return simplify(deriv);
}

// Integration: Term-by-term reverse power rule O(n)
function integrate(terms) {
    let integ = [];
    for (let t of terms) {
        integ.push(new Term(t.coeff / (t.exp + 1), t.exp + 1));
    }
    return simplify(integ);
}

// ==========================================
// UI & Plotting Logic
// ==========================================

function setPreset(expr) {
    document.getElementById('polyInput').value = expr;
    calculateAndPlot();
}

function calculateAndPlot() {
    const inputExpr = document.getElementById('polyInput').value.trim();
    if (!inputExpr) return;

    // 1. Process polynomial and calculus operations
    const terms = parsePolynomial(inputExpr);
    const deriv1 = differentiate(terms);
    const integ = integrate(terms);
    const degree = terms.length > 0 ? terms[0].exp : 0;

    const x0 = parseFloat(document.getElementById('evalX').value) || 0;
    const a = parseFloat(document.getElementById('intA').value) || 0;
    const b = parseFloat(document.getElementById('intB').value) || 0;
    const rMin = parseFloat(document.getElementById('rangeMin').value) || -5;
    const rMax = parseFloat(document.getElementById('rangeMax').value) || 5;

    // 2. Numerical evaluations (Horner's Method)
    const evalAtX0 = evaluatePoly(terms, x0);
    const defIntVal = evaluatePoly(integ, b) - evaluatePoly(integ, a);

    // 3. Update Result Display Cards
    document.getElementById('resExpanded').textContent = formatPolynomial(terms);
    document.getElementById('resDegree').textContent = degree;
    document.getElementById('resDeriv1').textContent = formatPolynomial(deriv1);
    document.getElementById('resIndefIntegral').textContent = formatPolynomial(integ) + " + C";
    document.getElementById('resEvalVal').textContent = `f(${x0}) = ${Math.round(evalAtX0 * 10000) / 10000}`;
    document.getElementById('resDefIntegral').textContent = Math.round(defIntVal * 10000) / 10000;

    // 4. Generate Data Points for Chart.js
    const xValues = [];
    const yPoly = [];
    const yDeriv = [];

    const step = (rMax - rMin) / 80.0;
    for (let x = rMin; x <= rMax + 1e-5; x += step) {
        let rx = Math.round(x * 100) / 100;
        xValues.push(rx);
        yPoly.push(evaluatePoly(terms, rx));
        yDeriv.push(evaluatePoly(deriv1, rx));
    }

    const ctx = document.getElementById('polyChart').getContext('2d');

    if (chartInstance) {
        chartInstance.destroy();
    }

    chartInstance = new Chart(ctx, {
        type: 'line',
        data: {
            labels: xValues,
            datasets: [
                {
                    label: `f(x) = ${formatPolynomial(terms)}`,
                    data: yPoly,
                    borderColor: '#58a6ff',
                    backgroundColor: 'rgba(88, 166, 255, 0.1)',
                    borderWidth: 2.5,
                    pointRadius: 1,
                    fill: false,
                    tension: 0.2
                },
                {
                    label: `f'(x) (Derivative) = ${formatPolynomial(deriv1)}`,
                    data: yDeriv,
                    borderColor: '#f85149',
                    borderWidth: 2,
                    borderDash: [6, 4],
                    pointRadius: 1,
                    fill: false,
                    tension: 0.2
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            interaction: {
                mode: 'index',
                intersect: false
            },
            plugins: {
                legend: {
                    labels: { color: '#c9d1d9', font: { size: 13 } }
                },
                tooltip: {
                    backgroundColor: '#161b22',
                    titleColor: '#58a6ff',
                    bodyColor: '#c9d1d9',
                    borderColor: '#30363d',
                    borderWidth: 1
                }
            },
            scales: {
                x: {
                    title: { display: true, text: 'x', color: '#8b949e' },
                    ticks: { color: '#8b949e', maxTicksLimit: 15 },
                    grid: { color: '#21262d' }
                },
                y: {
                    title: { display: true, text: 'y', color: '#8b949e' },
                    ticks: { color: '#8b949e' },
                    grid: { color: '#21262d' }
                }
            }
        }
    });
}

// Initial calculation on page load
window.onload = calculateAndPlot;
