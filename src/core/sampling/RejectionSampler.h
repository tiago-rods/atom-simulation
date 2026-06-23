#pragma once
#include "core/physics/ISampler.h"

class RejectionSampler : public ISampler {
public:
    std::vector<glm::vec3> sample(const HydrogenOrbital& orbital, int n) override;
};
