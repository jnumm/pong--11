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

#include "i18n.hpp"

namespace {

sf::String toSfString(std::string_view utf8String) {
  return sf::String::fromUtf8(begin(utf8String), end(utf8String));
}

} // namespace

Game::Game() : startMessage_{toSfString(_("Press space to start"))} {
  balls_[0].setVelocityRand(random_, true);

  window_.setFramerateLimit(60);

  circle_.setFillColor(sf::Color::Black);
  text_.setFillColor(sf::Color::Green);

  if (!font_.loadFromFile("font.ttf")) {
    throw NoFontException();
  }

#ifdef _WIN32
  auto hwnd{window_.getSystemHandle()};
  auto icon{LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON),
                      IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE)};
  SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
#endif
}

void Game::run1P() {
  sf::Event event;

  while (window_.isOpen()) {
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      } else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window_.close();
            break;
          case sf::Keyboard::Space:
            isRunning_ = !isRunning_;
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
  if (!isRunning_) {
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle1_.y > 0.f) {
    paddle1_.y -= Paddle::speed;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
             paddle1_.bottom() < height) {
    paddle1_.y += Paddle::speed;
  }

  updatePaddleAuto(paddle2_);

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
  for (const auto& ball : balls_) {
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

  for (auto& ball : balls_) {
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

    if (intersects(paddle1_, ball)) {
      ball.x = paddle1_.right();
      ball.flipXVelocity();

      toEnableBall = ball;
      toEnableBall.setVelocityRand(random_);
    } else if (intersects(paddle2_, ball)) {
      ball.setRight(paddle2_.x);
      ball.flipXVelocity();

      toEnableBall = ball;
      toEnableBall.setVelocityRand(random_);
      toEnableBall.flipXVelocity();
    }

    if (ball.right() < 0.f) {
      paddle2_.points += 1;
      ball.disable();
      nBallsEnabled_ -= 1;
    } else if (ball.x > width) {
      paddle1_.points += 1;
      ball.disable();
      nBallsEnabled_ -= 1;
    }
  }

  if (nBallsEnabled_ == 0) {
    toEnableBall = {width / 2.f - Ball::radius, height / 2.f - Ball::radius};
    toEnableBall.setVelocityRand(random_, true);
    isRunning_ = false;
  }

  if (!toEnableBall.isDisabled() && freePlace) {
    *freePlace = toEnableBall;
    nBallsEnabled_ += 1;
  }
}

void Game::render() {
  window_.clear(sf::Color::White);

  window_.draw(paddle1_.getRectangleShape());
  window_.draw(paddle2_.getRectangleShape());

  for (const auto& ball : balls_) {
    if (ball.isDisabled()) {
      continue;
    }

    circle_.setPosition(ball.x, ball.y);
    window_.draw(circle_);
  }

  text_.setString(std::to_string(paddle1_.points));
  text_.setPosition(width * 0.25f - text_.getGlobalBounds().width, 10.f);
  window_.draw(text_);

  text_.setString(std::to_string(paddle2_.points));
  text_.setPosition(width * 0.75f, 10.f);
  window_.draw(text_);

  if (!isRunning_) {
    text_.setString(startMessage_);
    text_.setCharacterSize(20);
    text_.setPosition(10.f, height - 30.f);
    window_.draw(text_);
    text_.setCharacterSize(50);
  }

  window_.display();
}
