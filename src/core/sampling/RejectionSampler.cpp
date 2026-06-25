#include "RejectionSampler.h"
#include "core/physics/HydrogenOrbital.h"
#include "core/math/SphericalCoords.h"
#include <cmath>
#include <random>

std::vector<glm::vec3> RejectionSampler::sample(const HydrogenOrbital& orbital, int numParticles) {
    int n = orbital.qn().n;
    double rMax = 3.0 * n * n;  // cobre ~99% da probabilidade em unidades de Bohr

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> distR(0.0, rMax);
    std::uniform_real_distribution<double> distTheta(0.0, M_PI);
    std::uniform_real_distribution<double> distPhi(0.0, 2.0 * M_PI);

    // Estima o máximo de f = |ψ|² × r² × sin(θ) amostrando 5000 pontos
    double M = 0.0;
    for (int i = 0; i < 5000; ++i) {
        double r     = distR(rng);
        double theta = distTheta(rng);
        double phi   = distPhi(rng);
        double f = orbital.psiSquared(r, theta, phi) * r * r * std::sin(theta);
        if (f > M) M = f;
    }
    M *= 1.1;  // margem de segurança para não rejeitar pontos válidos

    std::vector<glm::vec3> result;
    result.reserve(numParticles);

    std::uniform_real_distribution<double> distU(0.0, M);

    while ((int)result.size() < numParticles) {
        double r     = distR(rng);
        double theta = distTheta(rng);
        double phi   = distPhi(rng);

        double f = orbital.psiSquared(r, theta, phi) * r * r * std::sin(theta);

        if (distU(rng) < f)
            result.push_back(SphericalCoords::toCartesian(r, theta, phi));
    }

    return result;
}
