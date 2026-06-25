#pragma once
#include <vector>
#include <glm/glm.hpp>

class ParticleBuffer {
public:
    explicit ParticleBuffer(const std::vector<glm::vec3>& positions);
    ~ParticleBuffer();

    void upload(const std::vector<glm::vec3>& positions);
    void draw() const;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    int m_count = 0;
};
