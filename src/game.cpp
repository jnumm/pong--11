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

#include <algorithm>
#include <cmath>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

#include "config.hpp"
#include "direction.hpp"
#include "i18n.hpp"
#ifndef HAVE_STD_TO_STRING
#include "to_string_replacement.hpp"
#endif

const int Game::maxBalls = 30;

Game::Game()
: window_{{800, 600}, "Pong v" PONG_VERSION},
  font_{},
  text_{{}, font_, 50},
  startMessage_{},
  paddle1_{10.f, getHeight() / 2.f - 50.f},
  paddle2_{getWidth() - 30.f, getHeight() / 2.f - 50.f},
  balls_(1, {getWidth() / 2.f, getHeight() / 2.f, true, true}),
  isPlayer1Cpu_{false},
  isPlayer2Cpu_{false}
{
#ifdef _WIN32
  HWND hwnd = window_.getSystemHandle();
  HICON hicon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON),
                                 IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
  SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
#endif

  window_.setFramerateLimit(60);

  if (!font_.loadFromFile("font.ttf"))
    throw std::runtime_error{_("Failed to load font.")};

  const char* startMessageMb = _("Press SPACE to start");
  std::size_t len = std::mbstowcs(nullptr, startMessageMb, 0) + 1;
  std::vector<wchar_t> wstr(len);
  std::mbstowcs(wstr.data(), startMessageMb, wstr.size());
  startMessage_ = wstr.data();
}

void Game::run(bool isSingleplayer)
{
  isPlayer2Cpu_ = isSingleplayer;

  sf::Event event;

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
            for (Ball& ball : balls_)
              ball.start();
            break;
          default:
            break;
        }
      }
    }

    update();
    render();
  }
}

void Game::update()
{
  if (isPlayer1Cpu_)
    updatePaddleAuto(paddle1_);
  else
    updatePaddleControls(paddle1_, sf::Keyboard::W,  sf::Keyboard::S);

  if (isPlayer2Cpu_)
    updatePaddleAuto(paddle2_);
  else
    updatePaddleControls(paddle2_, sf::Keyboard::Up, sf::Keyboard::Down);

  bool shouldAddBall = false;
  float x, y;

  for (Ball& ball : balls_) {
    ball.updatePosition();

    if (ball.getTop() < 0.f || ball.getBottom() > getHeight()) {
      ball.bounceY();
      ball.updatePosition();
    }
    else if (paddle1_.getGlobalBounds().intersects(ball.getGlobalBounds())) {
      ball.setLeft(paddle1_.getRight());
      ball.bounceX();
      ball.updatePosition();
      shouldAddBall = true;
      x = ball.getLeft();
      y = ball.getTop();
    }
    else if (paddle2_.getGlobalBounds().intersects(ball.getGlobalBounds())) {
      ball.setRight(paddle2_.getLeft());
      ball.bounceX();
      ball.updatePosition();
      shouldAddBall = true;
      x = ball.getLeft();
      y = ball.getTop();
    }

    if (ball.getRight() < 0.f) {
      paddle2_.addPoints(1);
      ball.setPosition(getWidth() / 2.f, getHeight() / 2.f);
      ball.setRandomDirection();
      ball.stop();
    }
    else if (ball.getLeft() > getWidth()) {
      paddle1_.addPoints(1);
      ball.setPosition(getWidth() / 2.f, getHeight() / 2.f);
      ball.setRandomDirection();
      ball.stop();
    }
  }

  balls_.erase(std::remove_if(std::begin(balls_),
                              std::end(balls_),
                              [&](const Ball& ball) {
                                return balls_.size() > 1 && ball.isStopped();
                              }),
               std::end(balls_));

  if (shouldAddBall && balls_.size() < maxBalls) {
    balls_.emplace_back(x, y, false, true);
  }
}


void Game::updatePaddleControls(Paddle& paddle,
                                sf::Keyboard::Key upKey,
                                sf::Keyboard::Key downKey)
{
  if (sf::Keyboard::isKeyPressed(upKey) && paddle.getTop() > 0.f) {
    paddle.move(Direction::Up);
  }

  if (sf::Keyboard::isKeyPressed(downKey) && paddle.getBottom() < getHeight()) {
    paddle.move(Direction::Down);
  }
}

/*
 * This is the CPU player logic.
 * The paddle will only move if it is a certain distance away from the ball.
 */
void Game::updatePaddleAuto(Paddle& paddle)
{
  for (Ball& ball : balls_) {
    if (std::abs(ball.getLeft() - paddle.getLeft()) < 200.f) {
      if (paddle.getTop() > ball.getBottom() && paddle.getTop() > 0.f) {
        paddle.move(Direction::Up, Paddle::slowSpeed);
      }
      else if (paddle.getBottom() < ball.getTop() &&
               paddle.getBottom() < getHeight()) {
        paddle.move(Direction::Down, Paddle::slowSpeed);
      }
    }
  }
}

void Game::render()
{
  window_.clear(sf::Color::Black);

  window_.draw(paddle1_);
  window_.draw(paddle2_);
  for (const Ball& ball : balls_)
    window_.draw(ball);

  text_.setString(std::to_string(paddle1_.getPoints()));
  text_.setPosition(getWidth() * 0.25f - text_.getGlobalBounds().width, 10.f);
  window_.draw(text_);

  text_.setString(std::to_string(paddle2_.getPoints()));
  text_.setPosition(getWidth() * 0.75f, 10.f);
  window_.draw(text_);

  for (const Ball& ball : balls_) {
    if (ball.isStopped()) {
      text_.setString(startMessage_);
      text_.setCharacterSize(15);
      text_.setPosition(10.f, getHeight() - 25.f);
      window_.draw(text_);
      text_.setCharacterSize(50);
      break;
    }
  }

  window_.display();
}

float Game::getWidth() const
{
  return static_cast<float>(window_.getSize().x);
}

float Game::getHeight() const
{
  return static_cast<float>(window_.getSize().y);
}
