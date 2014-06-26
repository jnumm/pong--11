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

#include "game.hpp"

#include "config.hpp"
#include "direction.hpp"
#ifdef __MINGW32__
#include "to_string_replacement.hpp"
#endif

Game::Game()
: window_{{800, 600}, "Pong v" PONG_VERSION},
  font_{},
  text_{{}, font_, 50},
  paddle1_{10.f, getHeight() / 2.f - 50.f},
  paddle2_{getWidth() - 30.f, getHeight() / 2.f - 50.f},
  ball_{getWidth() / 2.f, getHeight() / 2.f}
{
  std::random_device rd;
  generator_ = std::make_shared<std::mt19937>(rd());
  ball_.setGenerator(generator_);

  window_.setFramerateLimit(60);

  ball_.setRandomDirection();

  font_.loadFromFile("font.ttf");
}

void Game::run()
{
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
  updatePaddleControls(paddle1_, sf::Keyboard::W,  sf::Keyboard::S);
  updatePaddleControls(paddle2_, sf::Keyboard::Up, sf::Keyboard::Down);


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

  if (sf::Keyboard::isKeyPressed(downKey) &&
      paddle.getBottom() < getHeight()) {
    paddle.move(Direction::Down);
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
    text_.setString("Press SPACE to start");
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
