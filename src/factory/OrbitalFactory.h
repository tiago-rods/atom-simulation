#pragma once
#include "IOrbitalFactory.h"

class OrbitalFactory : public IOrbitalFactory {
public:
    std::unique_ptr<HydrogenOrbital> createOrbital(int n, int l, int m) override;
    std::unique_ptr<ISampler>        createSampler() override;
};
