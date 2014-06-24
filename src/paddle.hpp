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

#ifndef PONG_PADDLE_HPP
#define PONG_PADDLE_HPP

#include <SFML/Graphics.hpp>

enum class Direction;

class Paddle : public sf::RectangleShape
{
public:
  static constexpr float width{20.f};
  static constexpr float height{100.f};
  static constexpr float velocity{20.f};

  Paddle(float x, float y);

  void move(Direction direction, float offset = velocity);

  float getRight() const;
  float getLeft() const;
  float getTop() const;
  float getBottom() const;

  void setPoints(int points);
  void addPoints(int pointsToAdd);
  int getPoints() const;

private:
  int points_;
};

#endif // PONG_PADDLE_HPP
