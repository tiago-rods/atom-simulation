  #include "CDFSampler.h"
  #include "core/physics/HydrogenOrbital.h"
  #include "core/math/SpecialFunctions.h"
  #include "core/math/SphericalCoords.h"
  #include <cmath>
  #include <random>
  #include <algorithm>
  #include <numeric>

static constexpr int N_R     = 1000;  // bins radiais
static constexpr int N_T     = 200;   // bins polares (CDF θ)
static constexpr int N_P     = 128;   // bins azimutais (CDF φ)
static constexpr int N_T_INT = 20;    // pontos de integração angular (para CDF radial)
static constexpr int N_P_INT = 16;

static void buildCDF(const std::vector<double>& pdf, std::vector<double>& cdf){
    cdf.resize((pdf.size()));
    std::partial_sum(pdf.begin(), pdf.end(), cdf.begin());
    double norm = cdf.back();

    for(auto& v : cdf) v /= norm;
}

double CDFSampler::invertCDF(const std::vector<double>& cdf, const std::vector<double>& bins, double u){
    auto it = std::lower_bound(cdf.begin(), cdf.end(), u);
    if(it == cdf.begin()) return bins.front();
    if(it == cdf.end()) return bins.back();

    size_t i = std::distance(cdf.begin(), it);
// interpolação linear entre dois bins diferentes
    double t = (u - cdf[i-1])/(cdf[i] - cdf[i-1]);
    return bins[i-1] + t * (bins[i] - bins[i -1 ]);
}

void CDFSampler::precompute(const HydrogenOrbital& orbital){
    int n = orbital.qn().n;
    int l = orbital.qn().l;
    int m = orbital.qn().m;
    double rMax = 3.0 * n * n;

    // ── CDF radial ──────────────────────────────────────────────────────────
      // p_r(r) ∝ r² × Σ_{j,k} psiSquared(r, θ_j, φ_k) × sinθ_j
      // A grade angular grosseira integra o fator angular → fica só r²|R|².
}