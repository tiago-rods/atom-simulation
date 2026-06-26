#include "ParticleBuffer.h"
#include <glad/gl.h>

ParticleBuffer::ParticleBuffer(const std::vector<glm::vec3>& positions, const std::vector<float>& probabilities){
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_vbo);
    upload(positions, probabilities);
}

ParticleBuffer::~ParticleBuffer() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_probVbo);
}

void ParticleBuffer::upload(const std::vector<glm::vec3>& positions, const std::vector<float>& probabilities) {
    m_count = static_cast<int>(positions.size());
    glBindVertexArray(m_vao);

     // Atributo 0: posição (vec3)
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(glm::vec3),
                   positions.data(), GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

      // Atributo 1: probabilidade (float)
      glBindBuffer(GL_ARRAY_BUFFER, m_probVbo);
      glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(float),
                   probabilities.data(), GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);

      glBindVertexArray(0);
}

void ParticleBuffer::draw() const {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_count);
    glBindVertexArray(0);
}
