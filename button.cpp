#include "button.hpp"

Button::Button(const std::string& title, const sf::Font &font, const sf::Vector2f &rect_size,
               const sf::Vector2f &position)
    : rect_{rect_size} {
  text_.setString(title);
  text_.setFont(font);
  text_.setCharacterSize(14);
  text_.setStyle(sf::Text::Bold);

  sf::Vector2f center = text_.getGlobalBounds().getSize() / 2.f;
  sf::Vector2f local_bounds = center + text_.getLocalBounds().getPosition();
  text_.setOrigin(local_bounds);

  text_.setPosition(position.x + rect_.getSize().x / 2,
                    position.y + rect_.getSize().y / 2);
  rect_.setPosition(position.x, position.y);
}

sf::RectangleShape &Button::getRect() { return this->rect_; }
sf::Text &Button::getText() { return this->text_; }

bool Button::mouseIsOver(const sf::RenderWindow &window) const {
  return this->rect_.getGlobalBounds().contains(
      sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
}

void Button::draw(sf::RenderWindow &window) {
  window.draw(this->rect_);
  window.draw(this->text_);
}
