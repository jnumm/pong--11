/* Copyright 2014 Juhani Numminen
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

#include "direction.hpp"

Paddle::Paddle(float x, float y)
: RectangleShape{{width, height}},
  points_{0}
{
  setPosition(x, y);
}

void Paddle::move(Direction direction, float offset)
{
  if (direction == Direction::Left)
    RectangleShape::move(-offset, 0.f);
  else if (direction == Direction::Right)
    RectangleShape::move(offset, 0.f);
  else if (direction == Direction::Up)
    RectangleShape::move(0.f, -offset);
  else if (direction == Direction::Down)
    RectangleShape::move(0.f, offset);
}

float Paddle::getLeft() const
{
  return getPosition().x;
}

float Paddle::getRight() const
{
  return getPosition().x + width;
}

float Paddle::getTop() const
{
  return getPosition().y;
}

float Paddle::getBottom() const
{
  return getPosition().y + height;
}

void Paddle::setPoints(int points)
{
  points_ = points;
}

void Paddle::addPoints(int pointsToAdd)
{
  points_ += pointsToAdd;
}

int Paddle::getPoints() const
{
  return points_;
}
