#pragma once
#include <stdexcept>

struct QuantumNumbers {
    int n; // principal: n >= 1
    int l; // angular:   0 <= l < n
    int m; // magnetic: -l <= m <= l

    static QuantumNumbers validate(int n, int l, int m) {
        if (n < 1)          throw std::invalid_argument("n >= 1");
        if (l < 0 || l >= n) throw std::invalid_argument("0 <= l < n");
        if (m < -l || m > l) throw std::invalid_argument("|m| <= l");
        return {n, l, m};
    }
};
