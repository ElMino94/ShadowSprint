#include "../../include/core/Game.h"
#include <cmath>
#include <iostream>

using namespace sf;
using namespace Utils;

Game::Game()
    : window(VideoMode({ 1920u, 1080u }), "ShadowSprint", Style::Default),
    currentState(MAINMENU),
    mainMenu(window), optionMenu(window), pauseMenu(window), igUI(window),
    player(3.f),
    font("../assets/fonts/samurai-blast.ttf"),
    countdownText(font, "3", 200),
    gameOverText(font, "GAME OVER", 150),
    gameStarted(false),
    gameOver(false),
    countdown(3.f),
    playerSpeed(1.f)
{
    window.setFramerateLimit(60);

    countdownText.setFillColor(Color::White);
    countdownText.setPosition(Vector2f(900.f, 300.f));

    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(Vector2f(600.f, 400.f));

    map = std::make_unique<Map>(window.getSize());
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<Event::Closed>())
            window.close();

        if (const auto* keyEvent = event->getIf<Event::KeyPressed>()) {
            if (keyEvent->scancode == Keyboard::Scancode::Escape) {
                if (currentState == PLAYING) {
                    currentState = PAUSEMENU;
                    pauseMenu.activate();
                }
                else if (currentState == PAUSEMENU) {
                    currentState = PLAYING;
                }
            }
        }

        // Menus
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
    case PAUSEMENU:
        pauseMenu.update(dt);
        break;
    case PLAYING: {
        float mapSpeed = map ? map->update(dt) : 0.f;

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
            }
        }
        else if (!gameOver) {
            player.handleInput();
            player.update(dt);

            sf::FloatRect bounds = player.getBounds();
            sf::Vector2f vel(0.f, player.getVelocityY());
            bool grounded = map->resolvePlayerCollisions(bounds, vel);
            if (grounded) player.setOnGround(true);
            else player.setOnGround(false);

            player.setVelocityY(vel.y);

            int hit = map->tryConsumePickup(player.getBounds());
            if (hit > 0) score += 100.f;
            else if (hit < 0) score = std::max(0.f, score - 50.f);

            playerSpeed = std::max(1.f, mapSpeed / 200.f);
            score += 2.f * playerSpeed * dt;
        }

        igUI.update(dt, score);
        break;
    }
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
    case PAUSEMENU:
        pauseMenu.draw(window);
        break;
    case PLAYING:
        if (map) map->draw(window);
        player.draw(window);
        if (!gameStarted && countdown > 0.f)
            window.draw(countdownText);
        if (gameOver)
            window.draw(gameOverText);
        igUI.draw(window);
        break;
    }

    window.display();
}

void Game::applyDisplaySettings(bool fullscreen, bool vsync) {
    VideoMode mode = fullscreen ? VideoMode::getDesktopMode() : VideoMode({ 1280u, 720u });
    unsigned int style = fullscreen ? Style::None : Style::Default;

    window.create(mode, "ShadowSprint", style);
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
    score = 0.f;
    player.reset();
    shurikens.clear();
    map = std::make_unique<Map>(window.getSize());
}
