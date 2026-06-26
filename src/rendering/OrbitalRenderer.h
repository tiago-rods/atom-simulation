#pragma once
#include "simulation/ISimulationObserver.h"
#include "ParticleBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include <memory>

class OrbitalRenderer : public ISimulationObserver {
public:
    OrbitalRenderer(int viewportW, int viewportH);

    void onParticlesUpdated(const std::vector<glm::vec3>& positions,
                            const std::vector<float>& probabilities) override;
    void draw(int viewportW, int viewportH);

    Camera& camera() { return m_camera; }

private:
    Camera                          m_camera;
    std::unique_ptr<Shader>         m_shader;
    std::unique_ptr<ParticleBuffer> m_buffer;
    float                           m_maxProb = 1.0f;
};
