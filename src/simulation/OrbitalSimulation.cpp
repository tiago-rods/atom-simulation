#include "OrbitalSimulation.h"

OrbitalSimulation::OrbitalSimulation(std::unique_ptr<HydrogenOrbital> orbital,
                                     std::unique_ptr<ISampler> sampler,
                                     int particleCount)
    : m_orbital(std::move(orbital))
    , m_sampler(std::move(sampler))
    , m_count(particleCount)
{}

void OrbitalSimulation::resample() {
    auto pts = m_sampler->sample(*m_orbital, m_count);
    notify(pts);
}

void OrbitalSimulation::addObserver(ISimulationObserver* obs) {
    m_observers.push_back(obs);
}

void OrbitalSimulation::notify(const std::vector<glm::vec3>& positions) {
    for (auto* obs : m_observers)
        obs->onParticlesUpdated(positions);
}
