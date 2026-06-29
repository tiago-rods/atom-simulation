#pragma once
#include "core/physics/ISampler.h"
#include "core/physics/QuantumNumbers.h"
#include <vector>

class CDFSampler : public ISampler {
public:
    std::vector<glm::vec3> sample(const HydrogenOrbital& orbital, int n) override;

private:
    void precompute(const HydrogenOrbital& orbital);
    static double invertCDF(const std::vector<double>& cdf, const std::vector<double>& bins, double u);

    std::vector<double> m_cdfR, m_rBins;
    std::vector<double> m_cdfTheta, m_thetaBins;
    std::vector<double> m_cdfPhi, m_phiBins;
    QuantumNumbers m_lastQN = { -1, 0, 0 }; // para saber quando recomputar

    int m_cachedN = -1;
    int m_cachedL = -1;
    int m_cachedM = -1;
};