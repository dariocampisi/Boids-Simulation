#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SFML/Graphics.hpp>

class Slider {
 public:
  Slider(const std::string &title, const sf::Font &font,
         const sf::Vector2f &line_size, const float dot_radius, const sf::Vector2f &position);

  const sf::RectangleShape &getLine() const;
  sf::CircleShape &getDot();
  sf::Text &getTitle();

  bool dotInRange() const;

  bool mouseIsOver(const sf::RenderWindow &window) const;

  bool dotLeft() const;
  bool dotRight() const;

  void linkTo() const;

  void work(const sf::RenderWindow &window, const bool mouse_pressed);

  void linkTo(float &f);

  void draw(sf::RenderWindow &window);

 private:
  sf::RectangleShape line_;
  sf::CircleShape dot_;
  sf::Text title_;
};  // definire una funzione per disegnare tutti gli slider insieme

#endif
