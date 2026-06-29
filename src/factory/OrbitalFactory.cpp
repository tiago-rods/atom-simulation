#include "OrbitalFactory.h"
#include "core/sampling/RejectionSampler.h"
#include "core/sampling/CDFSampler.h"

std::unique_ptr<HydrogenOrbital> OrbitalFactory::createOrbital(int n, int l, int m) {
    return std::make_unique<HydrogenOrbital>(QuantumNumbers::validate(n, l, m));
}

std::unique_ptr<ISampler> OrbitalFactory::createSampler() {
    return std::make_unique<CDFSampler>();
}
