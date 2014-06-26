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

#ifndef PONG_BALL_HPP
#define PONG_BALL_HPP

#include <random>

#include <SFML/Graphics.hpp>

#include "prng_user.hpp"

class Ball : public PrngUser, public sf::CircleShape
{
public:
  static constexpr float radius{10.f};
  static constexpr float velocity{10.f};

  Ball(float x, float y);

  void updatePosition();

  void setRandomDirection();

  void bounceX();
  void bounceY();

  void start();
  void stop();
  bool isStopped() const;

  float getLeft() const;
  void setLeft(float left);

  float getRight() const;  
  void setRight(float right);

  float getTop() const;
  void setTop(float top);

  float getBottom() const;
  void setBottom(float bottom);

private:
  std::uniform_real_distribution<float> distribution_;
  std::bernoulli_distribution bool_distribution_;

  sf::Vector2f velocityVector_;

  bool stopped_;
};

#endif // PONG_BALL_HPP
