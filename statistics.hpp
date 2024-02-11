#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include "boid.hpp"

namespace st {
float mean(const std::vector<float> &v);

float stdDev(const std::vector<float> &v, const float f);

void printStatistics(const std::vector<bd::Boid> &boids);
}  // namespace st

#endif
