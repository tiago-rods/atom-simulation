#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(float radius = 20.0f, float fovDeg = 45.0f);

    glm::mat4 view() const;
    glm::mat4 projection(float aspect) const;

    void onMouseDrag(float dx, float dy);
    void onScroll(float dy);

private:
    float m_radius;
    float m_yaw   = 0.0f;
    float m_pitch = 20.0f;
    float m_fov;
};
