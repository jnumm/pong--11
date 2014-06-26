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

#ifndef PONG_PRNG_USER_HPP
#define PONG_PRNG_USER_HPP

#include <memory>
#include <random>

class PrngUser
{
public:
  PrngUser(std::shared_ptr<std::mt19937> = nullptr);

  std::shared_ptr<std::mt19937> getGenerator() const;
  void setGenerator(std::shared_ptr<std::mt19937> generator);

protected:
  std::shared_ptr<std::mt19937> generator_;
};

#endif // PONG_PRNG_USER_HPP
