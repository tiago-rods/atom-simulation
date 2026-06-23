#pragma once
#include <glm/glm.hpp>

namespace SphericalCoords {
    // (r, theta, phi) -> cartesian (x, y, z)
    glm::vec3 toCartesian(double r, double theta, double phi);

    // (x, y, z) -> (r, theta, phi)
    struct Spherical { double r, theta, phi; };
    Spherical fromCartesian(const glm::vec3& v);
}
