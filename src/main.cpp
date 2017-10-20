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

#include <clocale>
#include <cstdio>
#include <cstring>
#include <string_view>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include <boost/format.hpp>

#include "config.hpp"
#include "game.hpp"
#include "i18n.hpp"

namespace {

void reportError(std::string_view msg) {
#ifdef _WIN32
  MessageBox(nullptr, msg.data(), _("Pong: Error"), MB_ICONERROR | MB_OK);
#else
  std::fputs(msg.data(), stderr);
#endif
}

} // namespace

int main(int argc, char* argv[]) {
  std::setlocale(LC_ALL, "");
#ifdef ENABLE_NLS
  bindtextdomain("pong--11", LOCALEDIR);
  bind_textdomain_codeset("pong--11", "UTF-8");
  textdomain("pong--11");
#endif

  auto isSingleplayer{false};

  if (argc > 1) {
    for (auto arg : std::vector<const char*>{argv + 1, argv + argc}) {
      if (std::strcmp(arg, "-s") == 0) {
        isSingleplayer = true;
      } else {
        reportError(str(boost::format(_("Invalid option '%1%'\n")) % arg));
        return EXIT_FAILURE;
      }
    }
  }

  try {
    // if (isSingleplayer)
    Game{}.run1P();
    // else
    //  Game{}.run2P();
  } catch (const Game::NoFontException&) {
    reportError(_("Error: Failed to load font.\n"));
    return EXIT_FAILURE;
  }
}
