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

#include <cmath> // For abs
#include <stdexcept>

#include "config.hpp"
#include "direction.hpp"
#include "i18n.hpp"
#ifdef __MINGW32__
#include "to_string_replacement.hpp"
#endif
#include "wchar_conversion.hpp"

Game::Game()
: window_{{800, 600}, "Pong v" PONG_VERSION},
  font_{},
  text_{{}, font_, 50},
  paddle1_{10.f, getHeight() / 2.f - 50.f},
  paddle2_{getWidth() - 30.f, getHeight() / 2.f - 50.f},
  ball_{getWidth() / 2.f, getHeight() / 2.f},
  player1_cpu_{false},
  player2_cpu_{false}
{
  std::random_device rd;
  generator_ = std::make_shared<std::mt19937>(rd());
  ball_.setGenerator(generator_);

  window_.setFramerateLimit(60);

  ball_.setRandomDirection();

  if (!font_.loadFromFile("font.ttf"))
    throw std::runtime_error{_("Failed to load font.")};
}

void Game::run(bool singleplayer)
{
  if (singleplayer)
    player2_cpu_ = true;

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
            ball_.start();
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
  if (!player1_cpu_)
    updatePaddleControls(paddle1_, sf::Keyboard::W,  sf::Keyboard::S);
  else
    updatePaddleAuto(paddle1_);

  if (!player2_cpu_)
    updatePaddleControls(paddle2_, sf::Keyboard::Up, sf::Keyboard::Down);
  else
    updatePaddleAuto(paddle2_);


  ball_.updatePosition();

  if (ball_.getTop() < 0.f || ball_.getBottom() > getHeight()) {
    ball_.bounceY();
  }
  else if (paddle1_.getGlobalBounds().intersects(ball_.getGlobalBounds())) {
    ball_.setLeft(paddle1_.getRight());
    ball_.bounceX();
  }
  else if (paddle2_.getGlobalBounds().intersects(ball_.getGlobalBounds())) {
    ball_.setRight(paddle2_.getLeft());
    ball_.bounceX();
  }
  else if (ball_.getRight() < 0.f) {
    paddle2_.addPoints(1);
    ball_.setPosition(getWidth() / 2.f, getHeight() / 2.f);
    ball_.setRandomDirection();
    ball_.stop();
  }
  else if (ball_.getLeft() > getWidth()) {
    paddle1_.addPoints(1);
    ball_.setPosition(getWidth() / 2.f, getHeight() / 2.f);
    ball_.setRandomDirection();
    ball_.stop();
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
  if (std::abs(ball_.getLeft() - paddle.getLeft()) < 200.f) {
    if (paddle.getTop() > ball_.getBottom() && paddle.getTop() > 0.f) {
      paddle.move(Direction::Up, Paddle::slow_velocity);
    }
    else if (paddle.getBottom() < ball_.getTop() &&
             paddle.getBottom() < getHeight()) {
      paddle.move(Direction::Down, Paddle::slow_velocity);
    }
  }
}

void Game::render()
{
  window_.clear(sf::Color::Black);

  window_.draw(paddle1_);
  window_.draw(paddle2_);
  window_.draw(ball_);

  text_.setString(std::to_string(paddle1_.getPoints()));
  text_.setPosition(getWidth() * 0.25f - text_.getGlobalBounds().width, 10.f);
  window_.draw(text_);

  text_.setString(std::to_string(paddle2_.getPoints()));
  text_.setPosition(getWidth() * 0.75f, 10.f);
  window_.draw(text_);

  if (ball_.isStopped()) {
    // Converted to const wchar_t* to make SFML display international characters
    // correctly.
    text_.setString(to_wstr(_("Press SPACE to start")));
    text_.setCharacterSize(15);
    text_.setPosition(10.f, getHeight() - 25.f);
    window_.draw(text_);
    text_.setCharacterSize(50);
  }

  window_.display();
}

float Game::getWidth() const
{
  return window_.getSize().x;
}

float Game::getHeight() const
{
  return window_.getSize().y;
}
