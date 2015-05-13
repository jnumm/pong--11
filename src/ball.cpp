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

#include "ball.hpp"

#include <cmath>

const float Ball::radius = 10.f;
const float Ball::speed = 10.f;

Ball::Ball(float x, float y)
: CircleShape{radius, 100},
  distribution_{speed * 0.5f, speed},
  boolDistribution_{0.5},
  velocityVector_{std::sqrt(speed * speed / 2.f),
                  std::sqrt(speed * speed / 2.f)},
  isStopped_{true}
{
  setOrigin(getRadius(), getRadius());
  setPosition(x, y);
}

Ball::Ball(float x, float y, bool stopped,
           std::shared_ptr<std::mt19937> generator, bool randomDirection)
: CircleShape{radius, 100},
  PrngUser{generator},
  distribution_{speed * 0.5f, speed},
  boolDistribution_{0.5},
  velocityVector_{std::sqrt(speed * speed / 2.f),
                  std::sqrt(speed * speed / 2.f)},
  isStopped_{stopped}
{
  setOrigin(getRadius(), getRadius());
  setPosition(x, y);

  if (randomDirection)
    setRandomDirection();
}

void Ball::updatePosition()
{
  if (!isStopped_)
    move(velocityVector_);
}

void Ball::setRandomDirection()
{
  float x{distribution_(*generator_)};
  float y{std::sqrt(speed * speed - x * x)};

  if (boolDistribution_(*generator_))
    x = -x;

  if (boolDistribution_(*generator_))
    y = -y;

  velocityVector_.x = x;
  velocityVector_.y = y;
}

void Ball::bounceX()
{
  float x{distribution_(*generator_)};
  float y{std::sqrt(speed * speed - x * x)};

  if (velocityVector_.x > 0.f)
    x = -x;

  if (velocityVector_.y < 0.f)
    y = -y;

  velocityVector_.x = x;
  velocityVector_.y = y;
}

void Ball::bounceY()
{
  velocityVector_.y = -velocityVector_.y;
}

void Ball::start()
{
  isStopped_ = false;
}

void Ball::stop()
{
  isStopped_ = true;
}

bool Ball::isStopped() const
{
  return isStopped_;
}

float Ball::getLeft() const
{
  return getPosition().x - getRadius();
}

void Ball::setLeft(float left)
{
  setPosition(left + getRadius(), getPosition().y);
}

float Ball::getRight() const
{
  return getPosition().x + getRadius();
}

void Ball::setRight(float right)
{
  setPosition(right - getRadius(), getPosition().y);
}

float Ball::getTop() const
{
  return getPosition().y - getRadius();
}

float Ball::getBottom() const
{
  return getPosition().y + getRadius();
}
