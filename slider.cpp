#include "slider.hpp"

Slider::Slider(const std::string &title, const sf::Font &font,
               const sf::Vector2f &line_size, const float dot_radius,
               const sf::Vector2f &position, float &parameter,
               const float initial_value)
    : line_{line_size},
      dot_{dot_radius},
      parameter_{parameter},
      initial_value_{initial_value} {
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

/*
bool Slider::dotInRange() const {
  return (this->dot_.getPosition().x >=
          (this->line_.getPosition().x - this->line_.getSize().x / 2)) &&
         (this->dot_.getPosition().x <=
          (this->line_.getPosition().x + this->line_.getSize().x / 2));
}
*/

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
  float step = this->line_.getSize().x / 11.f;

  if (this->mouseIsOver(window) && mouse_pressed) {
    this->dot_.setPosition(sf::Mouse::getPosition(window).x,
                           this->dot_.getPosition().y);

    float movement = this->dot_.getPosition().x - initial_position;
    int factor = static_cast<int>(movement / step);
    this->parameter_ =
        this->initial_value_ + factor * this->initial_value_ / 10.f;
  }
  this->normalize();
}

void Slider::draw(sf::RenderWindow &window) {
  window.draw(this->line_);
  window.draw(this->dot_);
  window.draw(this->title_);
}

void Slider::reset() {
  this->parameter_ = initial_value_;
  this->dot_.setPosition(this->line_.getPosition().x,
                         this->dot_.getPosition().y);
}

void Slider::setColor(const sf::Color &color) {
  this->line_.setFillColor(color);
  this->dot_.setFillColor(sf::Color(color.r - 15, color.g - 15, color.b - 15));
  this->title_.setFillColor(color);
}
