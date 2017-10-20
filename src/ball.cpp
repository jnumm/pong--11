/* Copyright 2017 Juhani Numminen
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

#include "random.hpp"

void Ball::move() {
  x += x_velocity;
  y += y_velocity;
}

void Ball::flipXVelocity() {
  x_velocity = -x_velocity;
}

void Ball::flipYVelocity() {
  y_velocity = -y_velocity;
}

void Ball::flipVelocityRand(Random& random) {
  if (random.randomBool()) {
    flipXVelocity();
  }

  if (random.randomBool()) {
    flipYVelocity();
  }
}

void Ball::setVelocityFromX(float new_x_velocity) {
  x_velocity = new_x_velocity;
  y_velocity = std::hypot(new_x_velocity, speed);
}

void Ball::setVelocityRand(Random& random, bool flipRandomly) {
  setVelocityFromX(random.randomFloat());
  if (flipRandomly) {
    flipVelocityRand(random);
  }
}

void Ball::disable() {
  x = disabled;
}

bool Ball::isDisabled() const {
  return x == disabled;
}

float Ball::right() const {
  return x + radius + radius;
}

float Ball::bottom() const {
  return y + radius + radius;
}

void Ball::setRight(float xForRight) {
  x = xForRight - radius - radius;
}

void Ball::setBottom(float yForBottom) {
  y = yForBottom - radius - radius;
}
