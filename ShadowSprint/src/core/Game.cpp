#include "../../include/core/Game.h"
#include <iostream>
#include <cmath>

namespace utils {
    inline bool intersectsAABB(const sf::FloatRect& a, const sf::FloatRect& b)
    {
        return (a.position.x < b.position.x + b.size.x) &&
            (a.position.x + a.size.x > b.position.x) &&
            (a.position.y < b.position.y + b.size.y) &&
            (a.position.y + a.size.y > b.position.y);
    }
}

Game::Game()
    : window(sf::VideoMode({ 1920u, 1080u }), "Runner 2D", sf::Style::None),
    mainMenu(window),
    optionMenu(window),
    player(3.f),
    font("../assets/fonts/samurai-blast.ttf"),
    countdownText(font, "3", 200),
    gameOverText(font, "GAME OVER", 150),
    gameStarted(false),
    gameOver(false),
    countdown(3.f),
    currentState(MAINMENU)
{
    window.setFramerateLimit(60);

    countdownText.setFillColor(sf::Color::White);
    countdownText.setPosition(sf::Vector2f(900.f, 300.f));

    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(sf::Vector2f(600.f, 400.f));
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->scancode == sf::Keyboard::Scancode::Escape)
                window.close();
        }

        if (currentState == MAINMENU) {
            MainMenu::MainMenuAction action = mainMenu.handleEvent(*event, window);
            switch (action) {
            case MainMenu::MainMenuAction::Play:
                currentState = PLAYING;
                resetGame();
                break;
            case MainMenu::MainMenuAction::Options:
                currentState = OPTIONSMENU;
                optionMenu.activate();
                break;
            case MainMenu::MainMenuAction::Quit:
                window.close();
                break;
            default:
                break;
            }
        }

        if (currentState == OPTIONSMENU) {
            OptionMenu::OptionAction action = optionMenu.handleEvent(*event, window);
            if (action == OptionMenu::OptionAction::Back) {
                currentState = MAINMENU;
                mainMenu.activate();
            }

            static bool lastVsync = optionMenu.isVsyncEnabled();
            bool currentVsync = optionMenu.isVsyncEnabled();
            if (currentVsync != lastVsync) {
                lastVsync = currentVsync;
                window.setVerticalSyncEnabled(currentVsync);
            }
        }
    }
}

void Game::update(float dt) {
    switch (currentState) {
    case MAINMENU:
        mainMenu.update(dt);
        break;

    case OPTIONSMENU:
        optionMenu.update(dt);
        break;

    case PLAYING: {
        if (!gameStarted) {
            countdown -= dt;
            player.setState(Player::State::Idle);

            if (countdown > 0.f) {
                countdownText.setString(std::to_string(static_cast<int>(std::ceil(countdown))));
            }
            else {
                countdown = 0.f;
                gameStarted = true;
                player.setState(Player::State::Running);

                shurikens.clear();
                auto bounds = player.getBounds();
                sf::Vector2f target = bounds.position + (bounds.size * 0.5f);
                shurikens.push_back(std::make_unique<Shuriken>(target));
            }
        }
        else if (!gameOver) {
            if (shurikenClock.getElapsedTime().asSeconds() > 1.5f) {
                shurikenClock.restart();
                auto bounds = player.getBounds();
                sf::Vector2f target = bounds.position + (bounds.size * 0.5f);
                shurikens.push_back(std::make_unique<Shuriken>(target));
            }

            for (auto it = shurikens.begin(); it != shurikens.end();) {
                (*it)->update(dt);
                bool remove = false;

                if (utils::intersectsAABB((*it)->getBounds(), player.getBounds())) {
                    if (player.isBlocking()) {
                        remove = true;
                    }
                    else {
                        gameOver = true;
                        player.setState(Player::State::Idle);
                        break;
                    }
                }

                if ((*it)->isOffScreen())
                    remove = true;

                if (remove)
                    it = shurikens.erase(it);
                else
                    ++it;
            }
        }

        if (!gameOver)
            player.handleInput();

        player.update(dt);
        break;
    }

    default:
        break;
    }
}

void Game::render() {
    window.clear();

    switch (currentState) {
    case MAINMENU:
        mainMenu.draw(window);
        break;

    case OPTIONSMENU:
        optionMenu.draw(window);
        break;

    case PLAYING: {
        player.draw(window);

        for (auto& s : shurikens)
            s->draw(window);

        if (!gameStarted && countdown > 0.f)
            window.draw(countdownText);

        if (gameOver)
            window.draw(gameOverText);

        break;
    }
    }

    window.display();
}

void Game::resetGame() {
    gameStarted = false;
    gameOver = false;
    countdown = 3.f;
    player.reset();
    shurikens.clear();
}
