#include "OrbitalSimulation.h"
#include "core/math/SphericalCoords.h"
#include<algorithm>
#include <cmath>

OrbitalSimulation::OrbitalSimulation(int n, int l, int m, int particleCount)
    : m_orbital(m_factory.createOrbital(n,l,m)), m_sampler(m_factory.createSampler()), m_count(particleCount)
{}

void OrbitalSimulation::resample() {
    //1 . pede ao sampler N posições distribuidas conforme |Psi|^2
    auto positions = m_sampler->sample(*m_orbital, m_count);
    //2. para cada posição, calcula |Psi|^2 - isso da a "cor" de cada particula
    std::vector<float> probs;
    probs.reserve(positions.size());
    for (const auto& p : positions){
        auto spherical = SphericalCoords::fromCartesian(p);
        float prob = static_cast<float>(m_orbital->psiSquared(spherical.r, spherical.theta, spherical.phi));
        probs.push_back(prob);
    }
    notify(positions, probs);
}

void OrbitalSimulation::ChangeN(int delta) {
    auto qn = m_orbital->qn();
    int l = std::clamp(qn.n + delta, 1, MAX_N);
    int l = std::min(qn.l, n, -1);// l < n 
    int m = std::clamp(qn.m, -l, l); // |m| <= l



}
void OrbitalSimulation::addObserver(ISimulationObserver* obs) {
    auto qn = m_orbital->qn();
}

void OrbitalSimulation::notify(const std::vector<glm::vec3>& positions) {
    for (auto* obs : m_observers)
        obs->onParticlesUpdated(positions);
}
