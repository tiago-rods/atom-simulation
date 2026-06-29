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

    //  CDF radial
      // p_r(r) ∝ r² × Σ_{j,k} psiSquared(r, θ_j, φ_k) × sinθ_j
      // A grade angular grosseira integra o fator angular → fica só r²|R|².

      m_rBins.resize(N_R);
      std::vector<double> pdfR(N_R);
      double dT_int = M_PI/N_T_INT;
      double dP_int = 2.0*M_PI/N_P_INT;

      for(int i = 0; i < N_R; ++i){
        m_rBins[i] = rMax * (i + 0.5)/N_R;
        double r = m_rBins[i];
        double sum = 0.0;
        for(int j = 0; j < N_T_INT; ++j){
            double theta = (j + 0.5) * dT_int;
            double sinT = std::sin(theta);
            for (int k = 0; k < N_P_INT; ++k){
                double phi = (k + 0.5) * dP_int;
                sum += orbital.psiSquared(r, theta, phi) *sinT;
            }

        }
        pdfR[i] = r*r*sum;
      }
      buildCDF(pdfR, m_cdfR);

       // CDF polar (thetaO) 

    m_thetaBins.resize(N_T);
    double dT = M_PI / N_T;
    std::vector<double> pdfTheta(N_T);
    for(int j = 0; j < N_T; ++j){
        double theta = (j + 0.5) * dT;
        double dl = SpecialFunctions::assocLegendre(l, std::abs(m), std::cos(theta));
        pdfTheta[j] = dl * dl * std::sin(theta);
        m_thetaBins[j] = theta;
    }
    buildCDF(pdfTheta, m_cdfTheta);

    // CDF azimutal (phi)

    m_phiBins.resize(N_P);
    std::vector<double> pdfPhi(N_P);
    double dP = 2.0 * M_PI / N_P;

    for(int k = 0; k < N_P; ++k){
        double phi = (k + 0.5) * dP;
        m_phiBins[k] = phi;
        if(m == 0) {
            pdfPhi[k] = 1.0;
        } else if (m > 0) {
            double c = std::cos(m * phi);
            pdfPhi[k] = c*c;
        } else {
            double s = std::sin(std::abs(m) * phi);
            pdfPhi[k] = s*s;
        }
    }
    buildCDF(pdfPhi, m_cdfPhi);
    m_cachedL = l;
    m_cachedN = n;
    m_cachedM = m;
}

std::vector<glm::vec3> CDFSampler::sample(const HydrogenOrbital& orbital, int numParticles){
    auto qn = orbital.qn();
    if(qn.n != m_cachedN || qn.l != m_cachedL || qn.m != m_cachedM) precompute(orbital);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<glm::vec3> result;
    result.reserve(numParticles);

    for(int i = 0; i < numParticles; ++i){
        double r = invertCDF(m_cdfR, m_rBins, dist(rng));
        double theta = invertCDF(m_cdfTheta, m_thetaBins, dist(rng));
        double phi = invertCDF(m_cdfPhi, m_phiBins, dist(rng));
        result.push_back(SphericalCoords::toCartesian(r,theta,phi));
    }
    return result;
}