#include "boid.hpp"

bd::Boid::Boid(const sf::Color &color, const sf::Vector2f &position,
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

bd::Boid::Boid(const sf::Vector2f &position, const sf::Vector2f &velocity)
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

sf::ConvexShape &bd::Boid::getShape() { return this->shape_; }
const sf::Vector2f &bd::Boid::getVelocity() const { return this->velocity_; }
const sf::Vector2f &bd::Boid::getPosition() const {
  return this->shape_.getPosition();
}

void bd::Boid::setVelocity(const float x, const float y) {
  this->velocity_ = sf::Vector2f(x, y);
}
void bd::Boid::setVelocity(const sf::Vector2f &v) { this->velocity_ = v; }

float bd::Boid::distance(const Boid &other) const {
  sf::Vector2f difference = this->getPosition() - other.getPosition();
  return std::hypot(difference.x, difference.y);
}

bool bd::Boid::isClose(const Boid &other, const float d) const {
  auto distance = this->distance(other);
  return distance < d;
}
bool bd::Boid::isCloseAndVisible(const Boid &other, const float d,
                                 const float angle_view) const {
  if (this->isClose(other, d)) {
    sf::Vector2f relative_position = other.getPosition() - this->getPosition();
    float relative_angle =
        std::atan2(relative_position.y, relative_position.x) * (180.f / M_PI);
    if (relative_angle < 0) {
      relative_angle += 360.f;
    }
    float angle_difference =
        std::abs(this->shape_.getRotation() - relative_angle);

    if (angle_difference <= (angle_view / 2)) {
      return 1;
    } else if (angle_difference >= ((angle_view / 2) + (360.f - angle_view))) {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

void bd::Boid::maxVelocity(const float max_velocity) {
  if (length(this->velocity_) > max_velocity) {
    normalize(this->velocity_);
    this->velocity_ *= max_velocity;
  }
}

void bd::Boid::avoidBoundary(const float window_width,
                             const float window_height, const float turn_factor,
                             const float margin) {
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

bool bd::Boid::isFlockMate(const Boid &other) const {
  return this->shape_.getFillColor() == other.shape_.getFillColor();
}

float bd::length(const sf::Vector2f &v) { return std::hypot(v.x, v.y); }

void bd::normalize(sf::Vector2f &v) {
  if (length(v) != 0) {
    v /= length(v);
  }
}
