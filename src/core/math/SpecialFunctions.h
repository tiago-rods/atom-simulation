#pragma once

namespace SpecialFunctions {
    // Associated Laguerre polynomial L_n^alpha(x)
    double assocLaguerre(int n, int alpha, double x);

    // Associated Legendre polynomial P_l^m(x), x in [-1, 1]
    double assocLegendre(int l, int m, double x);

    // Spherical harmonic Y_l^m(theta, phi) — real form
    double sphericalHarmonic(int l, int m, double theta, double phi);
}
