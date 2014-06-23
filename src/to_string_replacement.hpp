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

#ifndef TO_STRING_REPLACEMENT_HPP
#define TO_STRING_REPLACEMENT_HPP

#include <string>

#include <boost/lexical_cast.hpp>

namespace std
{

inline string to_string(int i)
{
  return boost::lexical_cast<string>(i);
}

}

#endif // TO_STRING_REPLACEMENT_HPP
