#include "OrbitalRenderer.h"
#include <glad/gl.h>
#include <algorithm>

OrbitalRenderer::OrbitalRenderer(int /*viewportW*/, int /*viewportH*/)
    : m_camera(20.0f)
{
    m_shader = std::make_unique<Shader>("shaders/particle.vert", "shaders/particle.frag");
}


void OrbitalRenderer::onParticlesUpdated(const std::vector<glm::vec3>& positions, const std::vector<float>& probabilities) {
    if (!m_buffer)
        m_buffer = std::make_unique<ParticleBuffer>(positions, probabilities);
    else 
        m_buffer->upload(positions, probabilities);

    m_maxProb = *std::max_element(probabilities.begin(), probabilities.end());
}

void OrbitalRenderer::draw(int viewportW, int viewportH) {
    if (!m_shader || !m_buffer) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->use();
    float aspect = static_cast<float>(viewportW) / static_cast<float>(viewportH);
    m_shader->setMat4("uView", m_camera.view());
    m_shader->setMat4("uProjection", m_camera.projection(aspect));
    m_shader->setFloat("uMaxProb", m_maxProb);

    m_buffer->draw();
}
