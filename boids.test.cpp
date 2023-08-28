#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "boid.hpp"
#include "doctest.h"
#include "slider.hpp"
#include "statistics.hpp"

TEST_CASE("Testing some critical functions") {
  SUBCASE("Testing Boid::isCloseAndVisible()") {
    Boid one{sf::Color::Black, sf::Vector2f(0.f, 0.f), sf::Vector2f(1.f, 0.f)};
    Boid two{sf::Color::Black, sf::Vector2f(20.f, 0.f), sf::Vector2f(1.f, 0.f)};
    CHECK(one.isCloseAndVisible(two, 150.f, 250.f));

    two.getShape().setPosition(sf::Vector2f(-10.f, 0.f));
    CHECK(!one.isCloseAndVisible(two, 150.f, 250.f));
    CHECK(one.isCloseAndVisible(two, 150.f, 360.f));

    two.getShape().setPosition(sf::Vector2f(151.f, 0.f));
    CHECK(!one.isCloseAndVisible(two, 150.f, 250.f));

    two.getShape().setPosition(sf::Vector2f(5.f, -2.f));
    CHECK(one.isCloseAndVisible(two, 150.f, 250.f));
  }

  SUBCASE("Testing Boid::isFlockMate()") {
    Boid one{sf::Color::Black, sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)};
    Boid two{sf::Color::Magenta, sf::Vector2f(100.f, 100.f),
             sf::Vector2f(-20.f, 10.f)};
    CHECK(!one.isFlockMate(two));

    two.getShape().setFillColor(sf::Color::Black);
    CHECK(one.isFlockMate(two));
  }

  SUBCASE("Testing Slider::reset()") {
    sf::Font font{};
    float parameter{100.f};

    Slider s{"test",
             font,
             sf::Vector2f(160.f, 2.5f),
             178.f,
             sf::Vector2f(500.f, 500.f),
             parameter,
             100.f};

    parameter += 50.f;

    s.reset();
    CHECK(parameter == 100.f);
  }

  SUBCASE("Testing printStatistics()") {
    Boid one{sf::Color::Black, sf::Vector2f(5.f, 2.f), sf::Vector2f(3.f, 6.f)};
    Boid two{sf::Color::Black, sf::Vector2f(6.f, 0.f),
             sf::Vector2f(10.f, -3.f)};
    Boid three{sf::Color::Black, sf::Vector2f(11.f, 12.f),
               sf::Vector2f(-6.f, -6.f)};
    Boid four{sf::Color::Black, sf::Vector2f(13.f, 0.f),
              sf::Vector2f(2.f, 1.f)};
    Boid five{sf::Color::Black, sf::Vector2f(1.f, 1.f),
              sf::Vector2f(-1.f, 12.f)};

    std::vector<Boid> boids{one, two, three, four, five};

    unsigned int frame_counter{39u};
    unsigned int frame_limit{40u};

    std::ostringstream oss;
    std::streambuf* originalOutput = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    printStatistics(frame_counter, frame_limit, boids);

    std::cout.rdbuf(originalOutput);

    CHECK(oss.str() ==
          "Mean distance: (9.04 +/- 2.31) px\nMean speed: (7.98 +/- "
          "3.39) px/frameTime\n\n");
  }
}
