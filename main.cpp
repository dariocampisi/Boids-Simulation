#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "boids.hpp"

int main() {
  // dimensioni finestra e font (arial)
  unsigned int window_width{1280};
  unsigned int window_height{720};
  sf::Font font;
  assert(font.loadFromFile("arial.ttf"));
  // ciao

  // parametri regole di volo
  float d{150.f};   // distanza minima perché due boid si considerino vicini
  float s{0.001f};  // forza separazione

  float d_s{20.f};  // distanza separazione
  assert(d_s < d);
  Slider s_sepa{"separation", font, sf::Vector2f(80.f, 2.5f), 7.f, 300.f, 35.f};

  float a{0.01f};  // allineamento
  assert(a < 1);
  Slider s_alig{"alignment", font, sf::Vector2f(80.f, 2.5f), 7.f, 500.f, 35.f};

  float c{0.0005f};  // coesione
  Slider s_cohe{"cohesion", font, sf::Vector2f(80.f, 2.5f), 7.f, 700.f, 35.f};

  float max_velocity{2.f};  // velocità massima

  // parametri predatore (p_)
  float p_d{60.f};  // distanza minima perché il predatore si consideri vicino
  float p_r{0.009f};  // repulsione predatore
  assert(p_r > s);
  float p_c{0.001f};  // coesione predatore
  assert(p_c > c);

  float p_max_velocity{2.5f};  // velocità massima predatore

  // parametri per evitare i bordi
  float margin{120.f};
  float turn_factor{0.03f};

  // angolo di vista
  float angle_view{250.f};

  // per mostrare gli fps
  sf::Text fps_text;
  fps_text.setFont(font);
  fps_text.setCharacterSize(21);
  fps_text.setFillColor(sf::Color::White);
  fps_text.setPosition(10.f, 10.f);
  sf::Clock clock;
  int frames = 0;
  sf::Time elapsed_time = sf::Time::Zero;

  // generazione numeri random
  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_real_distribution<float> rand_x_position(0.f + margin,
                                                        window_width - margin);
  std::uniform_real_distribution<float> rand_y_position(0.f + margin,
                                                        window_height - margin);
  std::uniform_real_distribution<float> rand_x_velocity(-2.5f, 2.5f);
  std::uniform_real_distribution<float> rand_y_velocity(-2.5f, 2.5f);

  // costruzione boids
  std::vector<Boid> boids;

  sf::Color light_blue(0, 102, 204);
  sf::Color orange(255, 128, 0);
  sf::Color green(0, 153, 0);
  std::vector<sf::Color> colors_vector;
  colors_vector.push_back(light_blue);
  colors_vector.push_back(orange);
  colors_vector.push_back(green);
  std::shuffle(colors_vector.begin(), colors_vector.end(), engine);

  std::uniform_int_distribution flocks(0, 2);
  int flocks_number = flocks(engine);
  std::uniform_int_distribution colors(0, flocks_number);

  for (int i = 0; i < 100; ++i) {
    int c = colors(engine);

    Boid boid{colors_vector[c],
              sf::Vector2f(rand_x_position(engine), rand_y_position(engine)),
              sf::Vector2f(rand_x_velocity(engine), rand_y_velocity(engine))};

    boids.push_back(boid);
  }

  // costruzione predatore
  Boid predator{sf::Vector2f(rand_x_position(engine), rand_y_position(engine)),
                sf::Vector2f(rand_x_velocity(engine), rand_y_velocity(engine))};

  // finestra e game loop
  sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Boids");
  window.setFramerateLimit(120);

  // per fuori focus
  bool window_in_focus = 1;
  sf::RectangleShape darkness{sf::Vector2f(window_width, window_height)};
  darkness.setFillColor(sf::Color(0, 0, 0, 50));

  // top bar
  sf::RectangleShape top_bar{sf::Vector2f(window_width, 50.f)};
  top_bar.setFillColor(sf::Color(255, 0, 0, 100));

  // per tasto sinistro premuto
  bool mouse_pressed = 0;

  while (window.isOpen()) {
    // gestione eventi
    sf::Event event;
    while (window.pollEvent(event)) {
      // per poter chiudere la finestra
      if (event.type == sf::Event::Closed) window.close();

      // mouse left-click fa spawnare un boid (ma non sulla top bar)
      if (!(sf::Mouse::getPosition(window).y <= 50.f)) {
        if (event.type == sf::Event::MouseButtonReleased) {
          if (event.mouseButton.button == sf::Mouse::Left) {
            int c = colors(engine);

            Boid boid{
                colors_vector[c],
                sf::Vector2f(sf::Mouse::getPosition(window).x,
                             sf::Mouse::getPosition(window).y),
                sf::Vector2f(rand_x_velocity(engine), rand_y_velocity(engine))};

            boids.push_back(boid);
          }
        }
      }

      // fuori focus
      if (event.type == sf::Event::GainedFocus) {
        window_in_focus = 1;
      }
      if (event.type == sf::Event::LostFocus) {
        window_in_focus = 0;
      }

      // tasto sinistro premuto rilasciato
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          mouse_pressed = 1;
        }
      }
      if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          mouse_pressed = 0;
        }
      }
    }

    if (window_in_focus) {
      // fps
      elapsed_time += clock.restart();
      ++frames;
      int fps{};
      if (elapsed_time >= sf::seconds(1.f)) {
        fps = frames / elapsed_time.asSeconds();
        fps_text.setString(std::to_string(fps) + " fps");

        frames = 0;
        elapsed_time = sf::Time::Zero;
      }

      // slider
      s_sepa.work(window, mouse_pressed);
      s_alig.work(window, mouse_pressed);
      s_cohe.work(window, mouse_pressed);

      // ciclo boids
      for (int i = 0; i < static_cast<int>(boids.size()); ++i) {
        // movimento
        boids[i].getShape().move(boids[i].getVelocity());
        boids[i].setRotation(
            std::atan2(boids[i].getVelocity().y, boids[i].getVelocity().x) *
            (180.f / M_PI));

        // controllo bordi e velocità
        boids[i].avoidBoundary(window_width, window_height, turn_factor,
                               margin);
        boids[i].maxVelocity(max_velocity);

        // regole di volo
        sf::Vector2f separation_sum{};
        sf::Vector2f alignment_sum{};
        sf::Vector2f cohesion_sum{};
        float n{0.f};  // numero di boid vicini

        for (int j = 0; j < static_cast<int>(boids.size()); ++j) {
          if (i == j) continue;
          if (boids[i].isCloseAndVisible(boids[j], d, angle_view)) {
            if (boids[i].isClose(boids[j], d_s)) {
              separation_sum += boids[j].getPosition() - boids[i].getPosition();
            }
            if (boids[i].isFlockMate(boids[j])) {
              ++n;
              alignment_sum += boids[j].getVelocity();
              cohesion_sum += boids[j].getPosition();
            }
          }
        }

        if (n == 0) continue;

        sf::Vector2f separation = -s * separation_sum;
        sf::Vector2f alignment = a * (alignment_sum / n);
        sf::Vector2f cohesion = c * (cohesion_sum / n - boids[i].getPosition());

        sf::Vector2f repulsion{};

        if (boids[i].isCloseAndVisible(predator, p_d, angle_view)) {
          repulsion = -p_r * (predator.getPosition() - boids[i].getPosition());
        }

        sf::Vector2f boid_new_velocity = boids[i].getVelocity() + separation +
                                         alignment + cohesion + repulsion;
        boids[i].setVelocity(boid_new_velocity);
      }

      // predator
      predator.getShape().move(predator.getVelocity());
      predator.setRotation(
          std::atan2(predator.getVelocity().y, predator.getVelocity().x) *
          (180.f / M_PI));

      predator.avoidBoundary(window_width, window_height, turn_factor, margin);
      predator.maxVelocity(p_max_velocity);

      sf::Vector2f p_cohesion_sum{};
      float p_n{0.f};
      for (int i = 0; i < static_cast<int>(boids.size()); ++i) {
        if (predator.isCloseAndVisible(boids[i], d, angle_view)) {
          ++p_n;
          p_cohesion_sum += boids[i].getPosition();
        }
      }
      if (p_n != 0) {
        sf::Vector2f p_cohesion =
            p_c * (p_cohesion_sum / p_n - predator.getPosition());
        sf::Vector2f p_new_velocity = predator.getVelocity() + p_cohesion;

        predator.setVelocity(p_new_velocity);
      }
    }

    // rendering
    sf::Color background_color(17, 17, 17);  // dark gray
    window.clear(background_color);

    for (int i = 0; i < static_cast<int>(boids.size()); ++i) {
      window.draw(boids[i].getShape());
    }
    window.draw(predator.getShape());
    window.draw(top_bar);
    window.draw(fps_text);

    s_sepa.draw(window);
    s_alig.draw(window);
    s_cohe.draw(window);

    if (!window_in_focus) window.draw(darkness);

    window.display();
  }
}
