// passare -DBUILD_TESTING=ON a cmake in fase di configurazione per abilitare i
// test edit: non credo sia necessario, lo fa di default
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "boid.hpp"
#include "doctest.h"

TEST_CASE("Testing some critical functions") {
  SUBCASE("Testing Boid::isCloseAndVisible()") {
    Boid one { sf::Color::Black , sf::Vector2f(500.f,500.f) , sf::Vector2f(1.f,0.f)};
    Boid two { sf::Color::Black , sf::Vector2f(520.f,500.f) , sf::Vector2f(1.f,0.f)};

    CHECK(one.isCloseAndVisible(two,50.f,250.f));
  }
}