#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include "boid.hpp"

float mean(const std::vector<float> &v);

float stdDev(const std::vector<float> &v, const float f);

void printStatistics(unsigned int &frame_counter,
                     const unsigned int frame_limit,
                     const std::vector<Boid> &boids);

#endif
