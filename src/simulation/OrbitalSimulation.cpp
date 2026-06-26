#include "OrbitalSimulation.h"
#include "core/math/SphericalCoords.h"
#include<algorithm>

static constexpr int MAX_N = 5;

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

void OrbitalSimulation::changeN(int delta) {
    auto qn = m_orbital->qn();
    int n = std::clamp(qn.n + delta, 1, MAX_N);
    int l = std::min(qn.l, n - 1);// l < n 
    int m = std::clamp(qn.m, -l, l); // |m| <= l
    m_orbital = m_factory.createOrbital(n, l, m);
    resample();
}

void OrbitalSimulation::changeL(int delta) {
    auto qn = m_orbital->qn();
    int l = std::clamp(qn.l + delta, 0, qn.l - 1);
    int m = std::clamp(qn.m, -l, l); 
    m_orbital = m_factory.createOrbital(qn.n, l, m);
    resample();
}

void OrbitalSimulation::changeM(int delta) {
      auto qn = m_orbital->qn();
      int m = std::clamp(qn.m + delta, -qn.l, qn.l);
      m_orbital = m_factory.createOrbital(qn.n, qn.l, m);
      resample();
  }


void OrbitalSimulation::addObserver(ISimulationObserver* obs) {
    m_observers.push_back(obs);
}

void OrbitalSimulation::notify(const std::vector<glm::vec3>& positions, const std::vector<float>& probs) {
    for (auto* obs : m_observers)
        obs->onParticlesUpdated(positions, probs);
}
