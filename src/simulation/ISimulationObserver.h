#pragma once
#include <vector>
#include <glm/glm.hpp>

class ISimulationObserver {
public:
    virtual ~ISimulationObserver() = default;
    virtual void onParticlesUpdated(const std::vector<glm::vec3>& positions,
                                    const std::vector<float>& probabilities) = 0; // Separa em 2 vetores pois position vai para um VBO 
                                    //e probabilities vai para outro VBO separado no OpenGL, dessa forma já fica no formato desejado
};
