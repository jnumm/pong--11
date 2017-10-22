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

#include "paddle.hpp"

#include "ball.hpp"

Paddle::Paddle(float x0, float y0, const sf::Color& color)
    : x{x0}, y{y0}, rectangleShape{{width, height}} {
  rectangleShape.setFillColor(color);
}

float Paddle::right() const {
  return x + width;
}

float Paddle::bottom() const {
  return y + height;
}

const sf::RectangleShape& Paddle::getRectangleShape() const {
  rectangleShape.setPosition(x, y);
  return rectangleShape;
}

bool intersects(const Paddle& paddle, const Ball& ball) {
  return (ball.right() > paddle.x) && (ball.x < paddle.right()) &&
         (ball.bottom() > paddle.y) && (ball.y < paddle.bottom());
}
