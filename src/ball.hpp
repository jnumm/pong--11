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

#ifndef PONG_BALL_HPP
#define PONG_BALL_HPP

#include <cmath>

class Ball
{
public:
  static constexpr auto radius   =  10.f;
  static constexpr auto speed    =  10.f;
  static constexpr auto disabled = -30.f;

  Ball(float x0 = disabled, float y0 = 0.f,
       float x_velocity0 = 0.f, float y_velocity0 = 0.f)
  : x{x0}, y{y0}, x_velocity{x_velocity0}, y_velocity{y_velocity0}
  { }

  static float fromOtherComponent(float a)
  {
    return std::sqrt(speed * speed - a * a);
  }

  float right()  const { return x + radius + radius; }
  float bottom() const { return y + radius + radius; }

  float x;
  float y;
  float x_velocity;
  float y_velocity;
};

#endif // PONG_BALL_HPP
