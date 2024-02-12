#include "statistics.hpp"

float st::mean(const std::vector<float> &v) {
  return std::accumulate(v.begin(), v.end(), 0.f) / v.size();
}

float st::stdDev(const std::vector<float> &v, const float f) {
  std::vector<float> quad_deviations{};

  for (auto it = v.begin(); it != v.end(); ++it) {
    quad_deviations.push_back((*it - f) * (*it - f));
  }

  return std::sqrt(mean(quad_deviations));
}

void st::printStatistics(const std::vector<bd::Boid> &boids) {
  // distanza media
  std::vector<float> relative_distances{};
  float mean_relative_distance{};
  std::vector<float> mean_relative_distances{};
  float mean_distance{};
  float std_dev_distance{};

  // stampa i risultati con una precisione di 2 cifre decimali (necessario per
  // implementare correttamente i test di st::printStatistics())
  std::cout << std::fixed << std::setprecision(2);

  for (unsigned int i = 0u; i < static_cast<unsigned int>(boids.size() - 1); ++i) {
    relative_distances.clear();
    for (unsigned int j = i + 1; j < static_cast<unsigned int>(boids.size());
         ++j) {
      if (!boids[i].isFlockMate(boids[j])) continue;
      relative_distances.push_back(
          bd::length(boids[i].getPosition() - boids[j].getPosition()));
    }
    mean_relative_distance = mean(relative_distances);
    mean_relative_distances.push_back(mean_relative_distance);
  }
  mean_distance = mean(mean_relative_distances);

  std_dev_distance = stdDev(mean_relative_distances, mean_distance);

  std::cout << "Mean distance: (" << mean_distance << " +/- "
            << std_dev_distance << ") px\n";

  // velocità media
  std::vector<float> speeds{};
  float mean_speed{};
  float std_dev_speed{};

  for (unsigned int i = 0u; i < static_cast<unsigned int>(boids.size()); ++i) {
    speeds.push_back(bd::length(boids[i].getVelocity()));
  }
  mean_speed = mean(speeds);

  std_dev_speed = stdDev(speeds, mean_speed);

  std::cout << "Mean speed: (" << mean_speed << " +/- " << std_dev_speed
            << ") px/frameTime\n\n";
}
