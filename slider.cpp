#include "slider.hpp"

Slider::Slider(const std::string &title, const sf::Font &font,
               const sf::Vector2f &line_size, const float dot_radius,
               const sf::Vector2f &position, float &f,
               const float initial_value)
    : line_{line_size}, dot_{dot_radius}, f_{f}, initial_value_{initial_value} {
  line_.setOrigin(line_size.x / 2, line_size.y / 2);
  dot_.setOrigin(dot_radius, dot_radius);

  title_.setString(title);
  title_.setFont(font);
  title_.setCharacterSize(12);
  title_.setFillColor(sf::Color::White);

  line_.setPosition(position.x, position.y);
  dot_.setPosition(position.x, position.y);
  title_.setPosition(position.x - line_.getSize().x / 2, position.y - 25.f);
}

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

bool Slider::dotTooLeft() const {
  return this->dot_.getPosition().x <
         (this->line_.getPosition().x - this->line_.getSize().x / 2);
}
bool Slider::dotTooRight() const {
  return this->dot_.getPosition().x >
         (this->line_.getPosition().x + this->line_.getSize().x / 2);
}
void Slider::normalize() {
  if (this->dotTooLeft()) {
    this->dot_.setPosition(
        this->line_.getPosition().x - this->line_.getSize().x / 2,
        this->dot_.getPosition().y);
  }
  if (this->dotTooRight()) {
    this->dot_.setPosition(
        this->line_.getPosition().x + this->line_.getSize().x / 2,
        this->dot_.getPosition().y);
  }
}

// il valore massimo assunto dal parametro è il doppio del valore iniziale
void Slider::work(const sf::RenderWindow &window, const bool mouse_pressed) {
  float initial_position = this->line_.getPosition().x;
  float step = this->line_.getSize().x / 21.f;

  if (this->mouseIsOver(window)) {
    if (mouse_pressed && this->dotInRange()) {
      this->dot_.setPosition(sf::Mouse::getPosition(window).x,
                             this->dot_.getPosition().y);

      float movement = this->dot_.getPosition().x - initial_position;
      int factor = static_cast<int>(movement / step);
      this->f_ = this->initial_value_ + factor * this->initial_value_ / 10.f;
    }
    this->normalize();
  }
}

void Slider::draw(sf::RenderWindow &window) {
  window.draw(this->line_);
  window.draw(this->dot_);
  window.draw(this->title_);
}

void Slider::reset() {
  this->f_ = initial_value_;
  this->dot_.setPosition(this->line_.getPosition().x,
                         this->dot_.getPosition().y);
}
