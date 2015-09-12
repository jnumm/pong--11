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
#include <exception>

#ifdef _WIN32
#include <windows.h>
#endif

#include "config.hpp"
#include "game.hpp"
#include "i18n.hpp"


int main(int argc, char** argv) {
  std::setlocale(LC_ALL, "");
#ifdef ENABLE_NLS
  bindtextdomain("pong--11", LOCALEDIR);
  textdomain("pong--11");
#endif

  bool isSingleplayer = false;

  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (std::strcmp(argv[i], "-s") == 0) {
        isSingleplayer = true;
      }
      else {
#ifdef _WIN32
        std::string err{_("Invalid option: ")};
        err = err + '\'' + argv[i] + '\'';
        MessageBox(NULL, err.c_str(), "Pong: Error", MB_ICONERROR | MB_OK);
#else
        std::fprintf(stderr, _("Invalid option: '%s'\n"), argv[i]);
#endif
        return EXIT_FAILURE;
      }
    }
  }

  try {
    Game{}.run(isSingleplayer);
  }
  catch (const std::exception& e) {
#ifdef _WIN32
    std::string err{_("Exception: ")};
    err += e.what();
    MessageBox(NULL, err.c_str(), "Pong: Error", MB_ICONERROR | MB_OK);
#else
    std::fprintf(stderr, _("Exception: %s\n"), e.what());
#endif
    return EXIT_FAILURE;
  }
}
