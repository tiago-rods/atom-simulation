#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera(float radius, float fovDeg)
    : m_radius(radius), m_fov(fovDeg) {}

glm::mat4 Camera::view() const {
    float yawR   = glm::radians(m_yaw);
    float pitchR = glm::radians(m_pitch);

    glm::vec3 eye{
        m_radius * std::cos(pitchR) * std::sin(yawR),
        m_radius * std::sin(pitchR),
        m_radius * std::cos(pitchR) * std::cos(yawR)
    };
    return glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::projection(float aspect) const {
    return glm::perspective(glm::radians(m_fov), aspect, 0.1f, 1000.0f);
}

void Camera::onMouseDrag(float dx, float dy) {
    m_yaw   += dx * 0.4f;
    m_pitch  = std::clamp(m_pitch - dy * 0.4f, -89.0f, 89.0f);
}

void Camera::onScroll(float dy) {
    m_radius = std::clamp(m_radius - dy * 1.5f, 1.0f, 200.0f);
}
