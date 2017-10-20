/* Copyright 2014-2017 Juhani Numminen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PONG_GAME_HPP
#define PONG_GAME_HPP

#include <array>

#include <SFML/Graphics.hpp>

#include "ball.hpp"
#include "config.hpp"
#include "paddle.hpp"
#include "random.hpp"

class Game {
 public:
  class NoFontException {};

  Game();

  void run1P();
  // void run2P();

 private:
  static constexpr auto width{800};
  static constexpr auto height{600};

  static constexpr auto nBalls{32};

  void update1P();
  // void update2P();
  void updatePaddleAuto(Paddle& paddle);
  void updateBalls();
  void render();

  std::array<Ball, nBalls> balls_{Ball{width / 2.f, height / 2.f}};
  int nBallsEnabled_ = 1;
  Paddle paddle1_{10.f, height / 2.f - 50.f, sf::Color::Blue};
  Paddle paddle2_{width - 30.f, height / 2.f - 50.f, sf::Color::Red};

  sf::RenderWindow window_{sf::VideoMode{static_cast<unsigned>(width),
                                         static_cast<unsigned>(height)},
                           "Pong v" PROJECT_VERSION};
  sf::CircleShape circle_{Ball::radius, 16};
  sf::Font font_;
  sf::Text text_{sf::String{}, font_, 50};
  const sf::String startMessage_;

  bool isRunning_{false};

  Random random_;
};

#endif // PONG_GAME_HPP
