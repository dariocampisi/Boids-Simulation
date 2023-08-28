#include "button.hpp"

bt::Button::Button(const std::string &title, const sf::Font &font,
               const sf::Vector2f &rect_size, unsigned int text_size,
               const sf::Vector2f &position)
    : rect_{rect_size} {
  rect_.setOrigin(rect_.getSize().x / 2, rect_.getSize().y / 2);
  rect_.setPosition(position.x, position.y);

  text_.setFont(font);
  text_.setString(title);
  text_.setCharacterSize(text_size);
  text_.setStyle(sf::Text::Bold);
  text_.setOrigin(text_.getGlobalBounds().getSize() / 2.f +
                  text_.getLocalBounds().getPosition());
  text_.setPosition(position.x, position.y);
}

sf::RectangleShape &bt::Button::getRect() { return this->rect_; }
sf::Text &bt::Button::getText() { return this->text_; }

bool bt::Button::mouseIsOver(const sf::RenderWindow &window) const {
  return this->rect_.getGlobalBounds().contains(
      sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
}

void bt::Button::draw(sf::RenderWindow &window) {
  window.draw(this->rect_);
  window.draw(this->text_);
}
