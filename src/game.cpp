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

#include "game.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include "config.hpp"
#include "i18n.hpp"
#include "random.hpp"
#ifndef HAVE_STD_TO_STRING
#include "to_string_replacement.hpp"
#endif

Game::Game()
: nBallsEnabled_{1},
  paddle1_{10.f, height / 2.f - 50.f},
  paddle2_{width - 30.f, height / 2.f - 50.f},
  window_{{width, height}, "Pong v" PONG_VERSION},
  circle_{Ball::radius, 16},
  text_{{}, font_, 50},
  isRunning_{false}
{
  balls_[0].x = width / 2.f;
  balls_[0].y = height / 2.f;
  balls_[0].x_velocity = randomFloat();
  balls_[0].y_velocity = Ball::fromOtherComponent(balls_[0].x_velocity);

  window_.setFramerateLimit(60);

  if (!font_.loadFromFile("font.ttf"))
    throw NoFontException();

  auto startMessageMb = _("Press space to start");
  auto len = std::mbstowcs(nullptr, startMessageMb, 0) + 1;
  auto wstr = std::vector<wchar_t>(len);
  std::mbstowcs(wstr.data(), startMessageMb, wstr.size());
  startMessage_ = wstr.data();

#ifdef _WIN32
  auto hwnd = window_.getSystemHandle();
  auto icon = LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON),
                        IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
  SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
#endif
}

void Game::run1P()
{
  auto event = sf::Event{};

  while (window_.isOpen()) {

    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      }
      else if (event.type == sf::Event::KeyPressed) {
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

void Game::update1P()
{
  // Control
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle1_.y > 0.f) {
    paddle1_.y -= Paddle::speed;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
           paddle1_.bottom() < height) {
    paddle1_.y += Paddle::speed;
  }

  if (!isRunning_) return;

  updatePaddleAuto(paddle2_);

  // Ball logic

  auto toEnableX = Ball::disabled;
  auto toEnableY = 0.f;
  auto toEnableXVel = 0.f;
  auto toEnableYVel = 0.f;
  auto toEnableIndex = indexDisabled;

  for (auto i = 0; i < nBalls; i++) {
    if (balls_[i].x == Ball::disabled) {
      if (toEnableIndex == indexDisabled) {
        toEnableIndex = i;
      }
      continue;
    }

    balls_[i].x += balls_[i].x_velocity;
    balls_[i].y += balls_[i].y_velocity;

    if (balls_[i].y < 0.f || balls_[i].bottom() > height) {
      balls_[i].y_velocity = -balls_[i].y_velocity;
    }

    if (intersects(paddle1_, balls_[i])) {
      balls_[i].x = paddle1_.right();
      balls_[i].x_velocity = -balls_[i].x_velocity;

      toEnableX = balls_[i].x;
      toEnableY = balls_[i].y;
      toEnableXVel = randomFloat();
      toEnableYVel = Ball::fromOtherComponent(toEnableXVel);
    }
    else if (intersects(paddle2_, balls_[i])) {
      balls_[i].x = paddle2_.x - Ball::radius*2;
      balls_[i].x_velocity = -balls_[i].x_velocity;

      toEnableX = balls_[i].x;
      toEnableY = balls_[i].y;
      toEnableXVel = -randomFloat();
      toEnableYVel = Ball::fromOtherComponent(toEnableXVel);
    }

    if (balls_[i].right() < 0.f) {
      paddle2_.points++;
      balls_[i].x = Ball::disabled;
      nBallsEnabled_--;
    }
    else if (balls_[i].x > width) {
      paddle1_.points++;
      balls_[i].x = Ball::disabled;
      nBallsEnabled_--;
    }
  }

  if (nBallsEnabled_ == 0) {
    toEnableX = width / 2.f - Ball::radius;
    toEnableY = height / 2.f - Ball::radius;
    toEnableXVel = randomFloat();
    toEnableYVel = Ball::fromOtherComponent(toEnableXVel);
    isRunning_ = false;
  }

  if (toEnableX != Ball::disabled && toEnableIndex != indexDisabled) {
    balls_[toEnableIndex].x = toEnableX;
    balls_[toEnableIndex].y = toEnableY;
    balls_[toEnableIndex].x_velocity = toEnableXVel;
    balls_[toEnableIndex].y_velocity = toEnableYVel;
    nBallsEnabled_++;
  }
}

//void Game::run2P()
//{
//}


//void Game::update2p()
//{
//}

/*
 * This is the CPU player logic.
 * The paddle will only move if it is a certain distance away from the ball.
 */
void Game::updatePaddleAuto(Paddle& paddle)
{
  for (const auto& ball : balls_) {
    if (ball.x == Ball::disabled) continue;

    if (std::abs(ball.x - paddle.x) < 200.f) {
      if (paddle.y > ball.y) {
        paddle.y -= Paddle::slowSpeed;
        return;
      }
      else if (paddle.bottom() < ball.bottom()) {
        paddle.y += Paddle::slowSpeed;
        return;
      }
    }
  }
}

void Game::render()
{
  window_.clear(sf::Color::Black);

  window_.draw(paddle1_.getRectangleShape());
  window_.draw(paddle2_.getRectangleShape());

  for (const auto& ball : balls_) {
    if (ball.x == Ball::disabled) continue;

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
