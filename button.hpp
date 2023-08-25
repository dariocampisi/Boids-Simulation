#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

#include "slider.hpp"

class Button {
 public:
  Button(const std::string& title, const sf::Font &font, const sf::Vector2f &rect_size,
         const sf::Vector2f &position);

  sf::RectangleShape &getRect();
  sf::Text &getText();

  bool mouseIsOver(const sf::RenderWindow &window) const;

  void draw(sf::RenderWindow &window);

 private:
  sf::RectangleShape rect_;
  sf::Text text_;
};

#endif
