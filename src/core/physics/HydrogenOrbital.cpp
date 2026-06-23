#include "HydrogenOrbital.h"

HydrogenOrbital::HydrogenOrbital(QuantumNumbers qn) : m_qn(qn) {}

double HydrogenOrbital::psi(double /*r*/, double /*theta*/, double /*phi*/) const {
    return 0.0; // Sprint 2
}

double HydrogenOrbital::psiSquared(double r, double theta, double phi) const {
    double p = psi(r, theta, phi);
    return p * p;
}
