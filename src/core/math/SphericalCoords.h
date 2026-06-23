#pragma once
#include <glm/glm.hpp>

namespace SphericalCoords {
    // Converte coordenadas esféricas (r, theta, phi) para cartesianas (x, y, z).
    // Convenção física: theta é o ângulo polar (0 a pi) medido do eixo Y, phi é o azimutal (0 a 2pi).
    glm::vec3 toCartesian(double r, double theta, double phi);

    struct Spherical { double r, theta, phi; };

    // Converte (x, y, z) para (r, theta, phi).
    // Retorna theta=0 quando r=0 para evitar divisão por zero.
    Spherical fromCartesian(const glm::vec3& v);
}
