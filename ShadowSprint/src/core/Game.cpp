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

using namespace sf;

Game::Game()
    : window(sf::VideoMode({ 1920u, 1080u }), "ShadowSprint", sf::Style::Default),
    currentState(MAINMENU),
    mainMenu(window), optionMenu(window), pauseMenu(window), igUI(window)
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
                if (currentState == PLAYING) {
                    currentState = PAUSEMENU;
                    pauseMenu.activate();
                }
                else if (currentState == PAUSEMENU) {
                    currentState = PLAYING;
                }
        }

        // MAIN MENU
        if (currentState == MAINMENU) {
            MainMenu::MainMenuAction action = mainMenu.handleEvent(*event);

            switch (action) {
            case MainMenu::MainMenuAction::Play:
                currentState = PLAYING;
                resetGame();
                break;
            case MainMenu::MainMenuAction::Options:
                currentState = OPTIONSMENU;
                optionMenu.setReturnContext(OptionMenu::ReturnContext::MainMenu);
                optionMenu.activate();
                break;
            case MainMenu::MainMenuAction::Quit:
                window.close();
                break;
            default:
                break;
            }
        }

        // OPTIONS MENU
        if (currentState == OPTIONSMENU) {
            OptionMenu::OptionAction action = optionMenu.handleEvent(*event);

            if (action == OptionMenu::OptionAction::Back) {
                bool fullscreen = optionMenu.isFullscreenEnabled();
                bool vsync = optionMenu.isVsyncEnabled();

                applyDisplaySettings(fullscreen, vsync);

                if (optionMenu.getReturnContext() == OptionMenu::ReturnContext::MainMenu) {
                    currentState = MAINMENU;
                    mainMenu.activate();
                }
                else {
                    currentState = PAUSEMENU;
                    pauseMenu.activate();
                }
            }
        }

        // PAUSE MENU
        if (currentState == PAUSEMENU) {
            PauseMenu::Action action = pauseMenu.handleEvent(*event);

            switch (action) {
            case PauseMenu::Action::Resume:
                currentState = PLAYING;
                break;
            case PauseMenu::Action::Options:
                currentState = OPTIONSMENU;
                optionMenu.setReturnContext(OptionMenu::ReturnContext::PauseMenu);
                optionMenu.activate();
                break;
            case PauseMenu::Action::Restart:
                resetGame();
                currentState = PLAYING;
                break;
            case PauseMenu::Action::BackToMenu:
                resetGame();
                currentState = MAINMENU;
                mainMenu.activate();
                break;
            case PauseMenu::Action::None:
            default:
                break;
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

                sf::FloatRect playerBounds = player.getBounds();
                sf::FloatRect shurikenBounds = (*it)->getBounds();

                if (utils::intersectsAABB(playerBounds, shurikenBounds)) {
                    if (player.isBlocking()) {
                        it = shurikens.erase(it);
                        continue;
                    }
                    else {
                        gameOver = true;
                        player.setState(Player::State::Idle);
                        break;
                    }
                }

                if ((*it)->isOffScreen())
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

    if (currentState == PAUSEMENU) {
        pauseMenu.update(dt);
    }

    if (currentState == PLAYING) {
        igUI.update(dt, 100.f);
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

        sf::FloatRect pb = player.getBounds();
        sf::RectangleShape playerBox(sf::Vector2f(pb.size));
        playerBox.setPosition(pb.position);
        playerBox.setFillColor(sf::Color(255, 0, 0, 80));
        window.draw(playerBox);

        for (auto& s : shurikens) {
            sf::FloatRect sb = s->getBounds();
            sf::RectangleShape shBox(sf::Vector2f(sb.size));
            shBox.setPosition(sb.position);
            shBox.setFillColor(sf::Color(0, 255, 0, 80));
            window.draw(shBox);
        }

        if (!gameStarted && countdown > 0.f)
            window.draw(countdownText);

        if (gameOver)
            window.draw(gameOverText);

        break;
    }
    }

    if (currentState == PAUSEMENU) {
        pauseMenu.draw(window);
    }

    if (currentState == PLAYING) {
        igUI.draw(window);
    }
    window.display();
}

void Game::applyDisplaySettings(bool fullscreen, bool vsync) {
    sf::VideoMode mode = fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode({ 1280, 720 });
    unsigned int style = fullscreen ? sf::Style::None : sf::Style::Default;

    window.create(mode, "Runner 2d", style);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(vsync);

    mainMenu.setFullscreen(fullscreen);
    optionMenu.setFullscreen(fullscreen);
    pauseMenu.setFullscreen(fullscreen);
    igUI.setFullscreen(fullscreen);
}

void Game::resetGame() {
    gameStarted = false;
    gameOver = false;
    countdown = 3.f;
    player.reset();
    shurikens.clear();
}
