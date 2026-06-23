#pragma once
#include <memory>
#include "core/physics/HydrogenOrbital.h"
#include "core/physics/ISampler.h"

class IOrbitalFactory {
public:
    virtual ~IOrbitalFactory() = default;
    virtual std::unique_ptr<HydrogenOrbital> createOrbital(int n, int l, int m) = 0;
    virtual std::unique_ptr<ISampler>        createSampler() = 0;
};
