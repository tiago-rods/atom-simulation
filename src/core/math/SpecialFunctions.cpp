#include "SpecialFunctions.h"

namespace SpecialFunctions {

double assocLaguerre(int n, int alpha, double x) {
    if (n == 0) return 1.0;
    double L_prev = 1.0; // L_0
    double L_curr = 1.0 + alpha - x; // L_1
    for(int i = 1; i < n; ++i) {
        double L_next = ((2 * i + 1.0 + alpha - x) * L_curr - (i + alpha) * L_prev) / (i + 1.0);
        L_prev = L_curr;
        L_curr = L_next;
    }
    return L_curr; // Sprint 2
}

double assocLegendre(int /*l*/, int /*m*/, double /*x*/) {
    return 0.0; // Sprint 2
}

double sphericalHarmonic(int /*l*/, int /*m*/, double /*theta*/, double /*phi*/) {
    return 0.0; // Sprint 2
}

}
