/* Copyright 2014-2015 Juhani Numminen
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

#include <SFML/Graphics.hpp>

#include "ball.hpp"
#include "paddle.hpp"

class Game
{
public:
  class NoFontException { };

  Game();

  void run1P();
//  void run2P();

private:
  static constexpr auto width  = 800u;
  static constexpr auto height = 600u;

  static constexpr auto nBalls = 32;
  static constexpr auto indexDisabled = -1;

  void update1P();
//  void update2P();
  void updatePaddleAuto(Paddle& paddle);
  void render();

  // Game objects
  Ball balls_[nBalls];
  int nBallsEnabled_;
  Paddle paddle1_;
  Paddle paddle2_;

  // Objects for drawing
  sf::RenderWindow window_;
  sf::CircleShape circle_;
  sf::Font font_;
  sf::Text text_;
  sf::String startMessage_;

  bool isRunning_;
};

#endif // PONG_GAME_HPP
