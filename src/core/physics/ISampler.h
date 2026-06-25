#pragma once
#include <vector>
#include <glm/glm.hpp>

class HydrogenOrbital;

class ISampler {
public:
    virtual ~ISampler() = default;
    virtual std::vector<glm::vec3> sample(const HydrogenOrbital& orbital, int n) = 0;
};
