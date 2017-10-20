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

#include "game.hpp"

#include <string_view>

#ifdef _WIN32
#include <windows.h>
#endif

#include "config.hpp"
#include "i18n.hpp"

namespace {

sf::String toSfString(std::string_view utf8String) {
  return sf::String::fromUtf8(begin(utf8String), end(utf8String));
}

} // namespace

Game::Game()
    : balls{Ball{width / 2.f, height / 2.f}},
      paddle1{10.f, height / 2.f - 50.f, sf::Color::Blue},
      paddle2{width - 30.f, height / 2.f - 50.f, sf::Color::Red},
      window{{static_cast<unsigned>(width), static_cast<unsigned>(height)},
             "Pong v" PROJECT_VERSION},
      circle{Ball::radius, 16},
      text{sf::String{}, font, 50},
      startMessage{toSfString(_("Press space to start"))} {
  balls[0].setVelocityRand(random, true);

  window.setFramerateLimit(60);

  circle.setFillColor(sf::Color::Black);
  text.setFillColor(sf::Color::Green);

  if (!font.loadFromFile("font.ttf")) {
    throw NoFontException();
  }

#ifdef _WIN32
  auto hwnd{window.getSystemHandle()};
  auto icon{LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON),
                      IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE)};
  SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
#endif
}

void Game::run1P() {
  sf::Event event;

  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;
          case sf::Keyboard::Space:
            isRunning = !isRunning;
            break;
          default:
            break;
        }
      }
    }

    update1P();
    render();
  }
}

void Game::update1P() {
  if (!isRunning) {
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle1.y > 0.f) {
    paddle1.y -= Paddle::speed;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
             paddle1.bottom() < height) {
    paddle1.y += Paddle::speed;
  }

  updatePaddleAuto(paddle2);

  updateBalls();
}

// void Game::run2P()
//{
//}

// void Game::update2p()
//{
//}

/*
 * This is the CPU player logic.
 * The paddle will only move if it is a certain distance away from the ball.
 */
void Game::updatePaddleAuto(Paddle& paddle) {
  for (const auto& ball : balls) {
    if (ball.isDisabled()) {
      continue;
    }

    if (std::abs(ball.x - paddle.x) < 200.f) {
      if (paddle.y > ball.y) {
        paddle.y -= Paddle::slowSpeed;
        return;
      }
      if (paddle.bottom() < ball.bottom()) {
        paddle.y += Paddle::slowSpeed;
        return;
      }
    }
  }
}

void Game::updateBalls() {
  Ball toEnableBall{};
  Ball* freePlace{nullptr};

  for (auto& ball : balls) {
    if (ball.isDisabled()) {
      if (!freePlace) {
        freePlace = &ball;
      }

      continue;
    }

    ball.move();

    if (ball.y < 0.f) {
      ball.y = 0.f;
      ball.flipYVelocity();
    } else if (ball.bottom() > height) {
      ball.setBottom(height);
      ball.flipYVelocity();
    }

    if (intersects(paddle1, ball)) {
      ball.x = paddle1.right();
      ball.flipXVelocity();

      toEnableBall = ball;
      toEnableBall.setVelocityRand(random);
    } else if (intersects(paddle2, ball)) {
      ball.setRight(paddle2.x);
      ball.flipXVelocity();

      toEnableBall = ball;
      toEnableBall.setVelocityRand(random);
      toEnableBall.flipXVelocity();
    }

    if (ball.right() < 0.f) {
      paddle2.points += 1;
      ball.disable();
      nBallsEnabled -= 1;
    } else if (ball.x > width) {
      paddle1.points += 1;
      ball.disable();
      nBallsEnabled -= 1;
    }
  }

  if (nBallsEnabled < 1) {
    toEnableBall = {width / 2.f - Ball::radius, height / 2.f - Ball::radius};
    toEnableBall.setVelocityRand(random, true);
    isRunning = false;
  }

  if (!toEnableBall.isDisabled() && freePlace) {
    *freePlace = toEnableBall;
    nBallsEnabled += 1;
  }
}

void Game::render() {
  window.clear(sf::Color::White);

  window.draw(paddle1.getRectangleShape());
  window.draw(paddle2.getRectangleShape());

  for (const auto& ball : balls) {
    if (ball.isDisabled()) {
      continue;
    }

    circle.setPosition(ball.x, ball.y);
    window.draw(circle);
  }

  text.setString(std::to_string(paddle1.points));
  text.setPosition(width * 0.25f - text.getGlobalBounds().width, 10.f);
  window.draw(text);

  text.setString(std::to_string(paddle2.points));
  text.setPosition(width * 0.75f, 10.f);
  window.draw(text);

  if (!isRunning) {
    text.setString(startMessage);
    text.setCharacterSize(20);
    text.setPosition(10.f, height - 30.f);
    window.draw(text);
    text.setCharacterSize(50);
  }

  window.display();
}
