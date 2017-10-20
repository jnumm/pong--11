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

#ifndef PONG_PADDLE_HPP
#define PONG_PADDLE_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Ball;

class Paddle {
 public:
  static constexpr auto width{20.f};
  static constexpr auto height{100.f};
  static constexpr auto speed{20.f};
  static constexpr auto slowSpeed{8.f};

  Paddle(float x0, float y0, const sf::Color& color);

  float right() const { return x + width; }
  float bottom() const { return y + height; }

  const sf::RectangleShape& getRectangleShape() const;

  float x{0.f};
  float y{0.f};
  int points{0};

 private:
  mutable sf::RectangleShape rectangleShape{{width, height}};
};

bool intersects(const Paddle& paddle, const Ball& ball);

#endif // PONG_PADDLE_HPP
