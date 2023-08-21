#include "button.hpp"

Button::Button(const sf::Font &font, const sf::Vector2f &rect_size,
               const sf::Vector2f &position)
    : rect_{rect_size} {
  text_.setString("RESET");
  text_.setFont(font);
  text_.setCharacterSize(14);
  text_.setFillColor(sf::Color(204,0,0));

  rect_.setFillColor(sf::Color(17,17,17));
  rect_.setOutlineColor(sf::Color(204,0,0));
  rect_.setOutlineThickness(1.f);

  rect_.setPosition(position.x, position.y);
  text_.setPosition(position.x, position.y);
}

bool Button::mouseIsOver(const sf::RenderWindow &window) const {
  return this->rect_.getGlobalBounds().contains(
      sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
}

void Button::clicked(Slider &slider) { slider.reset(); }

void Button::draw(sf::RenderWindow &window) {
  window.draw(this->rect_);
  window.draw(this->text_);
}
