#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SFML/Graphics.hpp>

class Slider {
 public:
  Slider(const std::string &title, const sf::Font &font,
         const sf::Vector2f &line_size, const float dot_radius,
         const sf::Vector2f &position, float &parameter, const float default_value);

  bool mouseIsOver(const sf::RenderWindow &window) const;

  bool dotTooLeft() const;
  bool dotTooRight() const;
  void stayInRange();

  void work(const sf::RenderWindow &window, const bool mouse_pressed);

  void draw(sf::RenderWindow &window);

  void reset();

 private:
  sf::RectangleShape line_;
  sf::CircleShape dot_;
  sf::Text title_;
  float &parameter_;
  const float default_value_;
};

#endif
