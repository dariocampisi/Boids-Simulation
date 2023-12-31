#include "slider.hpp"

sd::Slider::Slider(const std::string &title, const sf::Font &font,
                   const sf::Vector2f &line_size, const float dot_radius,
                   const sf::Vector2f &position, float &parameter,
                   const float default_value)
    : line_{line_size},
      dot_{dot_radius},
      parameter_{parameter},
      default_value_{default_value} {
  line_.setOrigin(line_size.x / 2, line_size.y / 2);
  dot_.setOrigin(dot_radius, dot_radius);

  title_.setFont(font);
  title_.setString(title);
  title_.setCharacterSize(15);

  line_.setPosition(position.x, position.y);
  dot_.setPosition(position.x, position.y);
  title_.setPosition(position.x - line_.getSize().x / 2, position.y - 28.f);
}

bool sd::Slider::mouseIsOver(const sf::RenderWindow &window) const {
  return this->dot_.getGlobalBounds().contains(
             sf::Vector2f(sf::Mouse::getPosition(window).x,
                          sf::Mouse::getPosition(window).y)) ||
         this->line_.getGlobalBounds().contains(
             sf::Vector2f(sf::Mouse::getPosition(window).x,
                          sf::Mouse::getPosition(window).y));
}

bool sd::Slider::dotTooLeft() const {
  return this->dot_.getPosition().x <
         (this->line_.getPosition().x - this->line_.getSize().x / 2);
}
bool sd::Slider::dotTooRight() const {
  return this->dot_.getPosition().x >
         (this->line_.getPosition().x + this->line_.getSize().x / 2);
}
void sd::Slider::stayInRange() {
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

// il valore massimo assunto dal parametro è 1.5 volte il valore iniziale
void sd::Slider::work(const sf::RenderWindow &window,
                      const bool mouse_pressed) {
  float initial_position = this->line_.getPosition().x;
  float step = this->line_.getSize().x / 11.f;

  if (this->mouseIsOver(window) && mouse_pressed) {
    this->dot_.setPosition(sf::Mouse::getPosition(window).x,
                           this->dot_.getPosition().y);

    float movement = this->dot_.getPosition().x - initial_position;
    int factor = static_cast<int>(movement / step);
    this->parameter_ =
        this->default_value_ + factor * this->default_value_ / 10.f;
  }
  this->stayInRange();
}

void sd::Slider::draw(sf::RenderWindow &window) {
  window.draw(this->line_);
  window.draw(this->dot_);
  window.draw(this->title_);
}

void sd::Slider::reset() {
  this->parameter_ = default_value_;
  this->dot_.setPosition(this->line_.getPosition().x,
                         this->dot_.getPosition().y);
}
