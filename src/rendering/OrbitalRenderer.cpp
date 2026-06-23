#include "OrbitalRenderer.h"
#include <glad/gl.h>

OrbitalRenderer::OrbitalRenderer(int /*viewportW*/, int /*viewportH*/)
    : m_camera(20.0f)
{
    // Shader and buffer are created after glad is loaded (Sprint 3)
}

void OrbitalRenderer::onParticlesUpdated(const std::vector<glm::vec3>& positions) {
    if (m_buffer)
        m_buffer->upload(positions);
}

void OrbitalRenderer::draw(int viewportW, int viewportH) {
    if (!m_shader || !m_buffer) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->use();
    float aspect = static_cast<float>(viewportW) / static_cast<float>(viewportH);
    m_shader->setMat4("uView",       m_camera.view());
    m_shader->setMat4("uProjection", m_camera.projection(aspect));

    m_buffer->draw();
}
