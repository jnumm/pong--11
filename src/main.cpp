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

#include <clocale>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

#include "config.hpp"
#include "game.hpp"
#include "i18n.hpp"


int main(int argc, char** argv)
{
  std::setlocale(LC_ALL, "");
#ifdef ENABLE_NLS
  bindtextdomain("pong--11", LOCALEDIR);
  textdomain("pong--11");
#endif

  auto isSingleplayer = false;

  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (std::strcmp(argv[i], "-s") == 0) {
        isSingleplayer = true;
      }
      else {
        const auto fmt = _("Invalid option: '%s'\n");
#ifdef _WIN32
        auto len = std::snprintf(nullptr, 0, fmt, argv[i]);
        auto buf = std::vector<char>(len + 1);
        std::snprintf(buf.data(), buf.size(), fmt, argv[i]);
        MessageBox(nullptr, buf, _("Pong: Error"), MB_ICONERROR | MB_OK);
#else
        std::fprintf(stderr, fmt, argv[i]);
#endif
        return EXIT_FAILURE;
      }
    }
  }

  try {
    //if (isSingleplayer)
      Game{}.run1P();
    //else
    //  Game{}.run2P();
  }
  catch (const Game::NoFontException&) {
    const auto msg = _("Error: Failed to load font.\n");
#ifdef _WIN32
    MessageBox(nullptr, msg, _("Pong: Error"), MB_ICONERROR | MB_OK);
#else
    std::fputs(msg, stderr);
#endif
    return EXIT_FAILURE;
  }
}
