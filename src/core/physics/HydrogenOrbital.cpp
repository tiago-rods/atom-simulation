#include "HydrogenOrbital.h"
#include <cmath>
#include "core/math/SpecialFunctions.h"

HydrogenOrbital::HydrogenOrbital(QuantumNumbers qn) : m_qn(qn) {}

double HydrogenOrbital::psi(double r, double theta, double phi) const {
    // calculo de rho
    int n = m_qn.n;
    int l = m_qn.l;
    int m = m_qn.m;

    double rho = 2.0 * r / n; // a_0 = 1 em unidades de Bohr
    // calcular norm
    double fatorEscala = (2.0/n) * (2.0/n) * (2.0/n); // (2/n)³
    double fatorNumerador = std::tgamma(n - l); // (n-l-1)!
    double fatorDenominador = 2.0 * n * std::tgamma(n + l + 1); // 2n (n+l)!

    double norm = std::sqrt(fatorEscala * fatorNumerador / fatorDenominador);

    // R x Y

    double radial = norm * std::exp(-rho / 2.0) * std::pow(rho,l) * SpecialFunctions::assocLaguerre(n - l - 1, 2 * l + 1, rho);
    double angular = SpecialFunctions::sphericalHarmonic(l, m, theta, phi);
    return radial * angular;
}

double HydrogenOrbital::psiSquared(double r, double theta, double phi) const {
    double p = psi(r, theta, phi);
    return p * p;
}
