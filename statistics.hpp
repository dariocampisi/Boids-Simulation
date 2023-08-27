#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <iostream>
#include <vector>

#include "boid.hpp"

inline float mean(const std::vector<float> &v);

inline float stdDev(const std::vector<float> &v, const float f);

void printStatistics(unsigned int frame_counter, const unsigned int frames_limit,
                const std::vector<Boid> &boids);

#endif