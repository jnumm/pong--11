/* Copyright 2015 Juhani Numminen
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

#ifndef WCHAR_CONVERSION_HPP
#define WCHAR_CONVERSION_HPP

#include <cwchar>
#include <vector>


/* Convert a multibyte string to a wide string. */
inline const wchar_t* to_wstr(const char* str)
{
    std::mbstate_t state = std::mbstate_t();
    int len = 1 + std::mbsrtowcs(nullptr, &str, 0, &state);
    std::vector<wchar_t> wstr(len);
    std::mbsrtowcs(wstr.data(), &str, wstr.size(), &state);
    return wstr.data();
}

#endif // WCHAR_CONVERSION_HPP
