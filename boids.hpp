#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class Boid : public sf::Transformable {
 public:
  // boids constructor
  Boid(const sf::Color &color, const sf::Vector2f &position,
       const sf::Vector2f &velocity);

  // predator constructor
  Boid(const sf::Vector2f &position, const sf::Vector2f &velocity);

  void setShape(const sf::ConvexShape &s);
  sf::ConvexShape &getShape();

  void setPosition(const float x, const float y);
  const sf::Vector2f &getPosition() const;

  void setVelocity(const float x, const float y);
  void setVelocity(const sf::Vector2f &v);
  const sf::Vector2f &getVelocity() const;

  void setRotation(const float r);
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
inline float length(const sf::Vector2f &v) { return std::hypot(v.x, v.y); }
inline void normalize(sf::Vector2f &v) {
  if (length(v) != 0) {
    v /= length(v);
  }
}

class Slider {
 public:
  Slider(const std::string &title, const sf::Font &font,
         const sf::Vector2f &line_size, const float dot_radius, const float x,
         const float y);

  const sf::RectangleShape &getLine() const;
  sf::CircleShape &getDot();
  sf::Text &getTitle();

  bool dotInRange() const;

  bool mouseIsOver(const sf::RenderWindow &window) const;

  bool dotLeft() const;
  bool dotRight() const;

  void linkTo() const;

  void work(const sf::RenderWindow &window, const bool mouse_pressed);

  void draw(sf::RenderWindow &window);

 private:
  sf::RectangleShape line_;
  sf::CircleShape dot_;
  sf::Text title_;
};  // definire una funzione per disegnare tutti gli slider insieme

#endif
