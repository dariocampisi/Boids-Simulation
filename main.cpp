#include <cassert>
#include <iostream>
#include <random>
#include <vector>

#include "boid.hpp"
#include "button.hpp"
#include "slider.hpp"

int main() {
  // DIMENSIONI FINESTRA E FONT
  unsigned int window_width{1280u};
  unsigned int window_height{720u};
  sf::Color background_color(17, 17, 17);  // grigio scuro
  sf::Font font{};                         // roboto
  assert(font.loadFromFile("utility/roboto.ttf"));

  // PARAMETRI E VARIABILI "GLOBALI"
  // parametri regole di volo
  float d{150.f};  // distanza minima perché due boid si considerino vicini
  Slider s_dist{"visual range",
                font,
                sf::Vector2f(160.f, 2.5f),
                7.f,
                sf::Vector2f(1000.f, 38.f),
                d,
                150.f};

  float s{0.0005f};  // separazione
  float d_s{20.f};   // distanza separazione
  assert(d_s < d);
  Slider s_sepa{"separation",
                font,
                sf::Vector2f(160.f, 2.5f),
                7.f,
                sf::Vector2f(250.f, 38.f),
                d_s,
                20.f};

  float a{0.01f};  // allineamento
  assert(a < 1);
  Slider s_alig{"alignment",
                font,
                sf::Vector2f(160.f, 2.5f),
                7.f,
                sf::Vector2f(500.f, 38.f),
                a,
                0.01f};

  float c{0.0005f};  // coesione
  Slider s_cohe{"cohesion",
                font,
                sf::Vector2f(160.f, 2.5f),
                7.f,
                sf::Vector2f(750.f, 38.f),
                c,
                0.0005f};

  float max_velocity{2.f};  // velocità massima

  // parametri predatore
  float p_r{0.009f};  // repulsione
  float p_d{60.f};    // distanza repulsione
  assert(p_r > s);
  float p_c{0.001f};  // coesione
  assert(p_c > c);

  float p_max_velocity{2.5f};  // velocità massima predatore

  // parametri per virare vicino ai bordi
  float margin{100.f};
  float turn_factor{0.25f};

  // angolo di visione
  float angle_view{250.f};

  // tasto sinistro del mouse premuto e non rilasciato
  bool mouse_pressed{0};

  // boid già costruiti
  bool boids_built{0};

  // troppi boid in input
  bool too_many_boids{0};

  // frame counter (per la gesitone dell'output delle statistiche)
  unsigned int frame_counter{0u};

  // GENERAZIONE NUMERI RANDOM (PER POSIZIONI E VELOCITÀ INIZIALI)
  std::random_device rd{};
  std::mt19937 engine(rd());
  std::uniform_real_distribution<float> rand_x_position(0.f + margin,
                                                        window_width - margin);
  std::uniform_real_distribution<float> rand_y_position(0.f + margin,
                                                        window_height - margin);
  std::uniform_real_distribution<float> rand_x_velocity(-2.f, 2.f);
  std::uniform_real_distribution<float> rand_y_velocity(-2.f, 2.f);

  // SCHERMATA INIZIALE
  bool start_clicked{0};

  sf::Text initial_text{};
  initial_text.setFont(font);
  initial_text.setString("Insert boids number ");
  initial_text.setCharacterSize(24);
  initial_text.setStyle(sf::Text::Bold);
  initial_text.setOrigin(initial_text.getGlobalBounds().getSize() / 2.f +
                         initial_text.getLocalBounds().getPosition());
  initial_text.setPosition(window_width / 2, window_height / 2 - 50.f);

  sf::Text num_boids{};
  num_boids.setFont(font);
  num_boids.setCharacterSize(30);
  num_boids.setStyle(sf::Text::Bold);
  num_boids.setPosition(window_width / 2, window_height / 2);

  sf::Text too_many{};
  too_many.setFont(font);
  too_many.setString("Too many! (300 max)");
  too_many.setCharacterSize(24);
  too_many.setStyle(sf::Text::Bold);
  too_many.setOrigin(too_many.getGlobalBounds().getSize() / 2.f +
                     too_many.getLocalBounds().getPosition());
  too_many.setPosition(window_width / 2, window_height / 2);

  Button start{"START", font, sf::Vector2f(170.f, 70.f), 20,
               sf::Vector2f(window_width / 2, window_height / 2 + 70.f)};
  start.getText().setFillColor(sf::Color(204, 0, 0));
  start.getRect().setFillColor(background_color);
  start.getRect().setOutlineColor(sf::Color(204, 0, 0));
  start.getRect().setOutlineThickness(2.f);

  std::string user_input{};

  // COSTRUZIONE BOID
  std::vector<Boid> boids{};

  // gestione di stormi e colori casuali
  sf::Color light_blue(0, 102, 204);
  sf::Color green(0, 153, 0);
  sf::Color orange(255, 128, 0);
  std::vector<sf::Color> colors_vector{};
  colors_vector.push_back(light_blue);
  colors_vector.push_back(orange);
  colors_vector.push_back(green);
  std::shuffle(colors_vector.begin(), colors_vector.end(), engine);

  std::uniform_int_distribution flocks(0, 2);
  int flocks_number = flocks(engine);
  std::uniform_int_distribution colors(0, flocks_number);

  // COSTRUZIONE PREDATORE
  Boid predator{sf::Vector2f(rand_x_position(engine), rand_y_position(engine)),
                sf::Vector2f(rand_x_velocity(engine), rand_y_velocity(engine))};

  // GESTIONE DELLA FINESTRA
  sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                          "Boids Simulation");
  window.setFramerateLimit(120);

  // per gestire il fuori focus
  bool window_in_focus{1};
  sf::RectangleShape darkness{sf::Vector2f(window_width, window_height)};
  darkness.setFillColor(sf::Color(0, 0, 0, 50));

  // top bar
  sf::RectangleShape top_bar{sf::Vector2f(window_width, 60.f)};
  top_bar.setFillColor(background_color);
  top_bar.setOutlineColor(colors_vector[0]);
  top_bar.setOutlineThickness(2.f);

  // fps
  sf::Text fps_text{};
  fps_text.setFont(font);
  fps_text.setCharacterSize(21);
  fps_text.setFillColor(sf::Color::White);
  fps_text.setPosition(15.f, 15.f);
  sf::Clock clock{};
  int frames = 0;
  sf::Time elapsed_time = sf::Time::Zero;

  // pulsante reset
  Button reset{"RESET", font, sf::Vector2f(80.f, 35.f), 14,
               sf::Vector2f(1200.f, 30.f)};
  reset.getText().setFillColor(colors_vector[0]);
  reset.getRect().setFillColor(background_color);
  reset.getRect().setOutlineColor(colors_vector[0]);
  reset.getRect().setOutlineThickness(1.f);

  // GAME LOOP
  while (window.isOpen()) {
    // GESTIONE EVENTI
    sf::Event event{};
    while (window.pollEvent(event)) {
      // chiusura finestra
      if (event.type == sf::Event::Closed) window.close();

      // gestione fuori focus
      if (event.type == sf::Event::GainedFocus) {
        window_in_focus = 1;
      }
      if (event.type == sf::Event::LostFocus) {
        window_in_focus = 0;
      }

      // SCHERMATA INIZIALE
      // testo iniziale
      if (event.type == sf::Event::TextEntered && event.text.unicode >= '0' &&
          event.text.unicode <= '9') {
        too_many_boids = 0;
        user_input += static_cast<char>(event.text.unicode);
        num_boids.setString(user_input);
        num_boids.setOrigin(num_boids.getGlobalBounds().getSize() / 2.f +
                            num_boids.getLocalBounds().getPosition());
      }
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::BackSpace && !user_input.empty()) {
        user_input.pop_back();
        num_boids.setString(user_input);
        num_boids.setOrigin(num_boids.getGlobalBounds().getSize() / 2.f +
                            num_boids.getLocalBounds().getPosition());
      }

      // pulsante start
      if ((start.mouseIsOver(window) &&
           event.type == sf::Event::MouseButtonPressed &&
           event.mouseButton.button == sf::Mouse::Left) ||
          (event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Enter)) {
        start.getText().setFillColor(background_color);
        start.getRect().setFillColor(sf::Color(204, 0, 0));
      }
      if (((start.mouseIsOver(window) &&
            event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left) ||
           (event.type == sf::Event::KeyReleased &&
            event.key.code == sf::Keyboard::Enter)) &&
          !user_input.empty()) {
        start.getText().setFillColor(sf::Color(204, 0, 0));
        start.getRect().setFillColor(background_color);

        if (std::stoi(user_input) <= 300) {
          start_clicked = 1;
        } else {
          too_many_boids = 1;

          user_input.clear();
          num_boids.setString(user_input);
          num_boids.setOrigin(num_boids.getGlobalBounds().getSize() / 2.f +
                              num_boids.getLocalBounds().getPosition());
        }
      }

      // mouse left-click genera un boid (non sulla top bar)
      if (!(sf::Mouse::getPosition(window).y <= top_bar.getSize().y)) {
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

      // tasto sinistro mouse premuto e non rilasciato
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

      // pulsantes reset
      if (reset.mouseIsOver(window) &&
          event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        reset.getText().setFillColor(background_color);
        reset.getRect().setFillColor(colors_vector[0]);
      }
      if (reset.mouseIsOver(window) &&
          event.type == sf::Event::MouseButtonReleased &&
          event.mouseButton.button == sf::Mouse::Left) {
        s_sepa.reset();
        s_alig.reset();
        s_cohe.reset();
        s_dist.reset();

        reset.getText().setFillColor(colors_vector[0]);
        reset.getRect().setFillColor(background_color);
      }
    }

    // GAME LOOP CORE
    if (start_clicked) {
      if (!boids_built && !user_input.empty()) {
        // costruzione boid (avviene solo una volta)
        for (unsigned int i = 0u;
             i < static_cast<unsigned int>(std::stoi(user_input)); ++i) {
          int c = colors(engine);

          Boid boid{
              colors_vector[c],
              sf::Vector2f(rand_x_position(engine), rand_y_position(engine)),
              sf::Vector2f(rand_x_velocity(engine), rand_y_velocity(engine))};

          boids.push_back(boid);
        }
        boids_built = 1;
      }
      if (window_in_focus) {
        // calcolo fps
        elapsed_time += clock.restart();
        ++frames;
        int fps{};
        if (elapsed_time >= sf::seconds(1.f)) {
          fps = frames / elapsed_time.asSeconds();
          fps_text.setString(std::to_string(fps) + " fps");

          frames = 0;
          elapsed_time = sf::Time::Zero;
        }

        // funzionamento slider
        s_sepa.work(window, mouse_pressed);
        s_alig.work(window, mouse_pressed);
        s_cohe.work(window, mouse_pressed);
        s_dist.work(window, mouse_pressed);

        // BOID
        for (unsigned int i = 0u; i < static_cast<unsigned int>(boids.size());
             ++i) {
          // movimento
          boids[i].getShape().move(boids[i].getVelocity());
          boids[i].getShape().setRotation(std::atan2(boids[i].getVelocity().y, boids[i].getVelocity().x) *
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

          for (unsigned int j = 0u; j < static_cast<unsigned int>(boids.size());
               ++j) {
            if (i == j) continue;
            if (boids[i].isCloseAndVisible(boids[j], d, angle_view)) {
              if (boids[i].isClose(boids[j], d_s)) {
                separation_sum +=
                    boids[j].getPosition() - boids[i].getPosition();
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
          sf::Vector2f cohesion =
              c * (cohesion_sum / n - boids[i].getPosition());

          sf::Vector2f repulsion{};

          if (boids[i].isCloseAndVisible(predator, p_d, angle_view)) {
            repulsion =
                -p_r * (predator.getPosition() - boids[i].getPosition());
          }

          sf::Vector2f boid_new_velocity = boids[i].getVelocity() + separation +
                                           alignment + cohesion + repulsion;
          boids[i].setVelocity(boid_new_velocity);
        }

        // PREDATORE
        // movimento, controllo ai bordi e velocità
        predator.getShape().move(predator.getVelocity());
        predator.setRotation(
            std::atan2(predator.getVelocity().y, predator.getVelocity().x) *
            (180.f / M_PI));

        predator.avoidBoundary(window_width, window_height, turn_factor,
                               margin);
        predator.maxVelocity(p_max_velocity);

        sf::Vector2f p_cohesion_sum{};
        float p_n{0.f};
        for (unsigned int i = 0u; i < static_cast<unsigned int>(boids.size());
             ++i) {
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

      // RENDERING
      window.clear(background_color);

      for (unsigned int i = 0u; i < static_cast<unsigned int>(boids.size());
           ++i) {
        window.draw(boids[i].getShape());
      }
      window.draw(predator.getShape());
      window.draw(top_bar);
      window.draw(fps_text);
      s_sepa.draw(window);
      s_alig.draw(window);
      s_cohe.draw(window);
      s_dist.draw(window);
      reset.draw(window);

      if (!window_in_focus) window.draw(darkness);

      window.display();

      /*
            // GESTIONE OUTPUT STATISTICHE
            ++frame_counter;

            if (frame_counter == 3'000u) {
              frame_counter = 0;

              // distanza media
              std::vector<float> relative_distances{};
              float mean_relative_distance{};
              std::vector<float> mean_relative_distances{};
              float mean_distance{};
              float std_dev_distance{};

              for (unsigned int i = 0u; i < static_cast<unsigned
         int>(boids.size());
                   ++i) {
                relative_distances.clear();

                for (unsigned int j = 0u; j < static_cast<unsigned
         int>(boids.size());
                     ++j) {
                  if (i == j) continue;
                  if (!boids[i].isFlockMate(boids[j])) continue;
                  relative_distances.push_back(
                      length(boids[i].getPosition() - boids[j].getPosition()));
                }
                mean_relative_distance = mean(relative_distances);
                mean_relative_distances.push_back(mean_relative_distance);
              }
              mean_distance = mean(mean_relative_distances);

              std_dev_distance = stdDev(mean_relative_distances, mean_distance);

              std::cout << "Mean distance: (" << mean_distance << " +/- "
                        << std_dev_distance << ") px \n";

              // velocità media
              std::vector<float> speeds{};
              float mean_speed{};
              float std_dev_speed{};

              for (unsigned int i = 0u; i < static_cast<unsigned
         int>(boids.size());
                   ++i) {
                speeds.push_back(length(boids[i].getVelocity()));
              }
              mean_speed = mean(speeds);

              std_dev_speed = stdDev(speeds, mean_speed);

              std::cout << "Mean speed: (" << mean_speed << " +/- " <<
         std_dev_speed
                        << ") px/frame";
            }
      */

    } else {
      window.clear(background_color);

      window.draw(initial_text);
      window.draw(num_boids);
      if (!user_input.empty()) start.draw(window);

      if (user_input.empty() && too_many_boids) window.draw(too_many);

      if (!window_in_focus) window.draw(darkness);

      window.display();
    }
  }
}
