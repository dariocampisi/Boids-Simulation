#include "slider.hpp"

Slider::Slider(const std::string &title, const sf::Font &font,
               const sf::Vector2f &line_size, const float dot_radius,
               const float x, const float y)
    : line_{line_size}, dot_{dot_radius} {
  line_.setOrigin(line_size.x / 2, line_size.y / 2);
  dot_.setOrigin(dot_radius, dot_radius);

  title_.setString(title);
  title_.setFont(font);
  title_.setCharacterSize(12);
  title_.setFillColor(sf::Color::White);

  line_.setPosition(x, y);
  dot_.setPosition(x, y);
  title_.setPosition(x - line_.getSize().x / 2, y - 25.f);
}

const sf::RectangleShape &Slider::getLine() const { return this->line_; }
sf::CircleShape &Slider::getDot() { return this->dot_; }
sf::Text &Slider::getTitle() { return this->title_; }

bool Slider::dotInRange() const {
  return (this->dot_.getPosition().x >=
          (this->line_.getPosition().x - this->line_.getSize().x / 2)) &&
         (this->dot_.getPosition().x <=
          (this->line_.getPosition().x + this->line_.getSize().x / 2));
}

bool Slider::mouseIsOver(const sf::RenderWindow &window) const {
  return this->dot_.getGlobalBounds().contains(
             sf::Vector2f(sf::Mouse::getPosition(window).x,
                          sf::Mouse::getPosition(window).y)) ||
         this->line_.getGlobalBounds().contains(
             sf::Vector2f(sf::Mouse::getPosition(window).x,
                          sf::Mouse::getPosition(window).y));
}

bool Slider::dotLeft() const {
  return this->dot_.getPosition().x <
         (this->line_.getPosition().x - this->line_.getSize().x / 2);
}
bool Slider::dotRight() const {
  return this->dot_.getPosition().x >
         (this->line_.getPosition().x + this->line_.getSize().x / 2);
}

void Slider::work(const sf::RenderWindow &window, const bool mouse_pressed) {
  if (this->mouseIsOver(window)) {
    if (mouse_pressed) {
      if (this->dotInRange()) {
        this->dot_.setPosition(sf::Mouse::getPosition(window).x,
                               this->dot_.getPosition().y);
      }
    }
    if (this->dotLeft()) {
      this->dot_.setPosition(
          this->line_.getPosition().x - this->line_.getSize().x / 2,
          this->dot_.getPosition().y);
    }
    if (this->dotRight()) {
      this->dot_.setPosition(
          this->line_.getPosition().x + this->line_.getSize().x / 2,
          this->dot_.getPosition().y);
    }
  }
}

void Slider::draw(sf::RenderWindow &window) {
  window.draw(this->line_);
  window.draw(this->dot_);
  window.draw(this->title_);
}
