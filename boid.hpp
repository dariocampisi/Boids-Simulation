#ifndef BOID_HPP
#define BOID_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <numeric>

namespace bd {
class Boid {
 public:
  // usato per i boid
  Boid(const sf::Color &color, const sf::Vector2f &position,
       const sf::Vector2f &velocity);

  // usato per il predatore
  Boid(const sf::Vector2f &position, const sf::Vector2f &velocity);

  sf::ConvexShape &getShape();
  const sf::Vector2f &getVelocity() const;
  const sf::Vector2f &getPosition() const;

  void setVelocity(const float x, const float y);
  void setVelocity(const sf::Vector2f &v);

  bool isClose(const Boid &other, const float d) const;
  bool isCloseAndVisible(const Boid &other, const float d,
                         const float angle_view) const;

  void maxVelocity(const float max_velocity);

  void avoidBoundary(const float window_width, const float window_height,
                     const float turn_factor, const float margin);

  bool isFlockMate(const Boid &other) const;

 private:
  sf::ConvexShape shape_;
  sf::Vector2f velocity_;

  float distance(const Boid &other) const;
};

// funzioni utili per bd::Boid::maxVelocity()
float length(const sf::Vector2f &v);

void normalize(sf::Vector2f &v);
}  // namespace bd

#endif
