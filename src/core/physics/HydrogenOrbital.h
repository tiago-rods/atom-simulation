#pragma once
#include "QuantumNumbers.h"

class HydrogenOrbital {
public:
    explicit HydrogenOrbital(QuantumNumbers qn);

    double psi(double r, double theta, double phi) const;
    double psiSquared(double r, double theta, double phi) const;

    const QuantumNumbers& qn() const { return m_qn; }

private:
    QuantumNumbers m_qn;
};
