#include "boids.hpp"

Boid::Boid(const sf::Color &color, const sf::Vector2f &position,
           const sf::Vector2f &velocity)
    : velocity_{velocity} {
  shape_.setPointCount(4);
  shape_.setPoint(0, sf::Vector2f(6.f, 0.f));
  shape_.setPoint(1, sf::Vector2f(-5.f, 4.f));
  shape_.setPoint(2, sf::Vector2f(-1.f, 0.f));
  shape_.setPoint(3, sf::Vector2f(-5.f, -4.f));
  shape_.setOrigin(0.f, 0.f);

  shape_.setFillColor(color);

  shape_.setPosition(position);
}

Boid::Boid(const sf::Vector2f &position, const sf::Vector2f &velocity)
    : velocity_{velocity} {
  shape_.setPointCount(4);
  shape_.setPoint(0, sf::Vector2f(8.f, 0.f));
  shape_.setPoint(1, sf::Vector2f(-7.f, 6.f));
  shape_.setPoint(2, sf::Vector2f(-3.f, 0.f));
  shape_.setPoint(3, sf::Vector2f(-7.f, -6.f));
  shape_.setOrigin(0.f, 0.f);
  shape_.setFillColor(sf::Color(204, 0, 0));

  shape_.setPosition(position);
}

void Boid::setShape(const sf::ConvexShape &s) { this->shape_ = s; }
sf::ConvexShape &Boid::getShape() { return this->shape_; }

void Boid::setPosition(const float x, const float y) {
  this->shape_.setPosition(x, y);
}
const sf::Vector2f &Boid::getPosition() const {
  return this->shape_.getPosition();
}

void Boid::setVelocity(const float x, const float y) {
  this->velocity_ = sf::Vector2f(x, y);
}
void Boid::setVelocity(const sf::Vector2f &v) { this->velocity_ = v; }
const sf::Vector2f &Boid::getVelocity() const { return this->velocity_; }

void Boid::setRotation(const float r) { this->shape_.setRotation(r); }
float Boid::getRotation() const { return this->shape_.getRotation(); }

float Boid::distance(const Boid &other) const {
  sf::Vector2f difference = this->getPosition() - other.getPosition();
  return std::hypot(difference.x, difference.y);
}

bool Boid::isClose(const Boid &other, const float d) const {
  auto distance = this->distance(other);
  return distance < d;
}
bool Boid::isCloseAndVisible(const Boid &other, const float d,
                             const float angle_view) const {
  auto distance = this->distance(other);
  if (distance < d) {
    sf::Vector2f relative_position = other.getPosition() - this->getPosition();
    float relative_angle =
        std::atan2(relative_position.y, relative_position.x) * (180.f / M_PI);
    if (relative_angle < 0) {
      relative_angle += 360.f;
    }
    float angle_difference = std::abs(this->getRotation() - relative_angle);

    return angle_difference <= (angle_view / 2);
  } else
    return 0;
}

void Boid::maxVelocity(const float max_velocity) {
  if (length(this->velocity_) > max_velocity) {
    normalize(this->velocity_);
    this->velocity_ *= max_velocity;
  }
}

void Boid::avoidBoundary(const float window_width, const float window_height,
                         const float turn_factor, const float margin) {
  if (this->getPosition().x < margin) {
    this->velocity_.x += turn_factor;
  }

  if (this->getPosition().x > (window_width - margin)) {
    this->velocity_.x -= turn_factor;
  }

  if (this->getPosition().y < margin) {
    this->velocity_.y += turn_factor;
  }

  if (this->getPosition().y > (window_height - margin)) {
    this->velocity_.y -= turn_factor;
  }
}

bool Boid::isFlockMate(const Boid &other) const {
  return this->shape_.getFillColor() == other.shape_.getFillColor();
}

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
