#pragma once
#include <memory>
#include <vector>
#include "ISimulationObserver.h"
#include "core/physics/HydrogenOrbital.h"
#include "core/physics/ISampler.h"

class OrbitalSimulation {
public:
    OrbitalSimulation(std::unique_ptr<HydrogenOrbital> orbital,
                      std::unique_ptr<ISampler> sampler,
                      int particleCount = 10000);

    void resample();
    void addObserver(ISimulationObserver* obs);

private:
    void notify(const std::vector<glm::vec3>& positions);

    std::unique_ptr<HydrogenOrbital> m_orbital;
    std::unique_ptr<ISampler>        m_sampler;
    int                              m_count;
    std::vector<ISimulationObserver*> m_observers;
};
