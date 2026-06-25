#include "SpecialFunctions.h"
#include <cmath>

static double semiFatorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; ++i){
        result *= i;
    }
    return result;
};
// podia usar a inline tgamma mas não sabia q existia vou deixar assim
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
    return L_curr;
}

double assocLegendre(int l, int m, double x) {
    int absM = std::abs(m);

    // 1. P_absM^absM via diagonal
    double pmm = 1.0;
    double somx2 = std::sqrt(1.0 - x * x);

    for(int k = 0; k < absM; ++k){
        pmm *= -(2 * k + 1) * somx2;
    }

    if(l == absM){
        if(m < 0) pmm *= std::pow(-1.0, absM) * semiFatorial(l - absM) / semiFatorial(l + absM);
        return pmm;
    }

    // 2. sobe até P_l^absM
    double pmm1 = x * (2 * absM + 1) * pmm;
    if (l == absM + 1) {
        if(m < 0) pmm1 *= std::pow(-1.0, absM) * semiFatorial(l - absM) / semiFatorial(l + absM);
        return pmm1;
    }

    double pll = 0.0;
    for (int k = absM + 1; k < l; ++k){
        pll = ((2 * k + 1) * x * pmm1 - (k + absM) * pmm) / (k - absM + 1);
        pmm = pmm1;
        pmm1 = pll;
    }
    pll = ((2 * l) * x * pmm1 - (l + absM - 1) * pmm) / (l - absM);

    if(m < 0) pll *= std::pow(-1.0, absM) * semiFatorial(l - absM) / semiFatorial(l + absM);
    return pll;
}

double sphericalHarmonic(int l, int m, double theta, double phi) {
    int absM = std::abs(m);

    double norm = std::sqrt((2.0 * l + 1) / (4.0 * M_PI) * semiFatorial(l - absM) / semiFatorial(l + absM));
    double plm = assocLegendre(l, absM, std::cos(theta));

    if (m > 0) return std::sqrt(2.0) * norm * plm * std::cos(absM * phi);
    if (m < 0) return std::sqrt(2.0) * norm * plm * std::sin(absM * phi);
    return norm * plm; // m == 0
}

}
