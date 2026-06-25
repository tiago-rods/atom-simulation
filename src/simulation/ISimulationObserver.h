#pragma once
#include <vector>
#include <glm/glm.hpp>

class ISimulationObserver {
public:
    virtual ~ISimulationObserver() = default;
    virtual void onParticlesUpdated(const std::vector<glm::vec3>& positions) = 0;
};
