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

  std::array<Ball, nBalls> balls;
  int nBallsEnabled{1};
  Paddle paddle1;
  Paddle paddle2;

  sf::RenderWindow window;
  sf::CircleShape circle;
  sf::Font font;
  sf::Text text;
  const sf::String startMessage;

  bool isRunning{false};

  Random random;
};

#endif // PONG_GAME_HPP
