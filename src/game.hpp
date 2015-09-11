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
  Game();

  void run(bool isSingleplayer);

private:
  void update();
  void updatePaddleControls(Paddle& paddle,
                            sf::Keyboard::Key upKey,
                            sf::Keyboard::Key downKey);
  void updatePaddleAuto(Paddle& paddle);
  void render();

  float getWidth() const;
  float getHeight() const;

  static const int maxBalls;

  sf::RenderWindow window_;

  sf::Font font_;
  sf::Text text_;
  sf::String startMessage_;

  Paddle paddle1_;
  Paddle paddle2_;
  std::vector<Ball> balls_;

  bool isPlayer1Cpu_, isPlayer2Cpu_;
};

#endif // PONG_GAME_HPP
