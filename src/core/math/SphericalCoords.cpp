#include "SphericalCoords.h"
#include <cmath>

namespace SphericalCoords {

glm::vec3 toCartesian(double r, double theta, double phi) {
    return {
        static_cast<float>(r * std::sin(theta) * std::cos(phi)),
        static_cast<float>(r * std::cos(theta)),
        static_cast<float>(r * std::sin(theta) * std::sin(phi))
    };
}

Spherical fromCartesian(const glm::vec3& v) {
    double r     = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    double theta = (r > 0.0) ? std::acos(v.y / r) : 0.0;
    double phi   = std::atan2(v.z, v.x);
    return {r, theta, phi};
}

}
