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

#ifndef PONG_RANDOM_HPP
#define PONG_RANDOM_HPP

#include <random>

#include "ball.hpp"

class Random {
 public:
  float randomFloat() { return fDist(engine); }
  bool randomBool() { return bDist(engine); }

 private:
  std::mt19937 engine{std::random_device{}()};
  std::uniform_real_distribution<float> fDist{Ball::speed * 0.5f, Ball::speed};
  std::bernoulli_distribution bDist{0.5};
};

#endif // PONG_RANDOM_HPP
