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

#ifndef PONG_BALL_HPP
#define PONG_BALL_HPP

class Random;

class Ball {
 public:
  static constexpr auto radius{10.f};
  static constexpr auto speed{10.f};
  static constexpr auto disabled{-30.f};

  void move();

  void flipXVelocity();
  void flipYVelocity();
  void flipVelocityRand(Random& random);

  void setVelocityFromX(float new_x_velocity);
  void setVelocityRand(Random& random, bool flipRandomly = false);

  void disable();
  bool isDisabled() const;

  float right() const;
  float bottom() const;

  void setRight(float xForRight);
  void setBottom(float yForBottom);

  float x{disabled};
  float y{0.f};
  float x_velocity{0.f};
  float y_velocity{0.f};
};

#endif // PONG_BALL_HPP
