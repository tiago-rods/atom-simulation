#pragma once

namespace SpecialFunctions {
    // Polinômio de Laguerre associado L_n^alpha(x).
    // Aparece na parte radial da função de onda: R_{n,l}(r) usa L_{n-l-1}^{2l+1}(rho).
    double assocLaguerre(int n, int alpha, double x);

    // Polinômio de Legendre associado P_l^m(x), x em [-1, 1].
    // Aparece nos harmônicos esféricos: Y_l^m(theta, phi) usa P_l^m(cos theta).
    double assocLegendre(int l, int m, double x);

    // Harmônico esférico Y_l^m(theta, phi) na forma real.
    // Combina P_l^m(cos theta) com cos(m*phi) ou sin(m*phi) dependendo do sinal de m.
    double sphericalHarmonic(int l, int m, double theta, double phi);
}
