#ifndef BOID_HPP
#define BOID_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <numeric>

class Boid : public sf::Transformable {
 public:
  // boids constructor
  Boid(const sf::Color &color, const sf::Vector2f &position,
       const sf::Vector2f &velocity);

  // predator constructor
  Boid(const sf::Vector2f &position, const sf::Vector2f &velocity);

  void setShape(const sf::ConvexShape &s);
  sf::ConvexShape &getShape();

  void setVelocity(const float x, const float y);
  void setVelocity(const sf::Vector2f &v);
  const sf::Vector2f &getVelocity() const;

  const sf::Vector2f &getPosition() const;
  float getRotation() const;

  float distance(const Boid &other) const;

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
};

// funzioni utili per Boid::maxVelocity()
float length(const sf::Vector2f &v);

void normalize(sf::Vector2f &v);

// funzioni utili per la stampa delle statistiche
float mean(const std::vector<float> &v);

float stdDev(const std::vector<float> &v, const float f);

#endif
