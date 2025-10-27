#include "../../include/core/Game.h"
#include <iostream>
#include <cmath>

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
    countdown(3.f), playerSpeed(1.f)
{
    window.setFramerateLimit(60);

    countdownText.setFillColor(Color::White);
    countdownText.setPosition(Vector2f(900.f, 300.f));

    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(Vector2f(600.f, 400.f));

    if (!slowBonusTexture.loadFromFile("../assets/textures/bonus_slowmode.png"))
        std::cerr << "Erreur chargement bonus_slowmode.png\n";
    if (!invincibilityBonusTexture.loadFromFile("../assets/textures/bonus_invincibility.png"))
        std::cerr << "Erreur chargement bonus_invincibility.png\n";
    if (!scoreBonusTexture.loadFromFile("../assets/textures/bonus_x2.png"))
        std::cerr << "Erreur chargement bonus_x2.png\n";
}

void Game::run() {
    Clock clock;
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
            if (keyEvent->scancode == Keyboard::Scancode::Escape)
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

        case PAUSEMENU:
            pauseMenu.update(dt);
            break;

        case PLAYING: {
            if (bonusSpawnClock.getElapsedTime().asSeconds() > bonusSpawnInterval) {
                bonusSpawnClock.restart();
                spawnRandomBonus();
            }

            for (auto it = activeBonuses.begin(); it != activeBonuses.end();) {
                (*it)->update(dt);

                if (intersectsAABB(player.getBounds(), (*it)->getBounds())) {
                    (*it)->apply(player);

                    if (dynamic_cast<InvincibilityBonus*>(it->get()))
                        igUI.addBonusIcon(invincibilityBonusTexture, 5.f);
                    else if (dynamic_cast<SlowModeBonus*>(it->get()))
                        igUI.addBonusIcon(slowBonusTexture, 5.f);
                    else if (dynamic_cast<ScoreMultiplierBonus*>(it->get()))
                        igUI.addBonusIcon(scoreBonusTexture, 5.f);

                    it = activeBonuses.erase(it);
                }
                else if ((*it)->isOffScreen()) {
                    it = activeBonuses.erase(it);
                }
                else {
                    ++it;
                }
            }

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
                    Vector2f target = bounds.position + (bounds.size * 0.5f);
                    shurikens.push_back(std::make_unique<Shuriken>(target));
                }
            }
            else if (!gameOver) {
                if (shurikenClock.getElapsedTime().asSeconds() > 1.5f) {
                    shurikenClock.restart();
                    auto bounds = player.getBounds();
                    Vector2f target = bounds.position + (bounds.size * 0.5f);
                    shurikens.push_back(std::make_unique<Shuriken>(target));
                }

                for (auto it = shurikens.begin(); it != shurikens.end();) {
                    float shurikenSpeedFactor = player.isSlowMode() ? 0.5f : 1.f;
                    (*it)->update(dt * shurikenSpeedFactor);

                    FloatRect playerBounds = player.getBounds();
                    FloatRect shurikenBounds = (*it)->getBounds();

                    if (intersectsAABB(playerBounds, shurikenBounds)) {
                        if (player.isBlocking() || player.isInvincible()) {
                            it = shurikens.erase(it);
                            continue;
                        }
                        else {
                            //gameOver = true;
                            break;
                        }
                    }

                    if ((*it)->isOffScreen())
                        it = shurikens.erase(it);
                    else
                        ++it;
                }
            }

            if (!gameOver && gameStarted) {
                player.handleInput();
                score += 2.f * playerSpeed * dt * player.getScoreMultiplier();
            }

            igUI.update(dt, score);
            player.update(dt);
            player.updateBonusTimer(dt);

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

        case PLAYING: {
            player.draw(window);

            for (auto& s : shurikens)
                s->draw(window);

            FloatRect pb = player.getBounds();
            RectangleShape playerBox(Vector2f(pb.size));
            playerBox.setPosition(pb.position);
            playerBox.setFillColor(Color(255, 0, 0, 80));
            window.draw(playerBox);

            for (auto& s : shurikens) {
                FloatRect sb = s->getBounds();
                RectangleShape shBox(Vector2f(sb.size));
                shBox.setPosition(sb.position);
                shBox.setFillColor(Color(0, 255, 0, 80));
                window.draw(shBox);
            }

            for (const auto& bonus : activeBonuses)
                bonus->draw(window);

            if (!gameStarted && countdown > 0.f)
                window.draw(countdownText);

            if (gameOver)
                window.draw(gameOverText);

            igUI.draw(window);

            break;
        }
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
    activeBonuses.clear();
}

void Game::spawnRandomBonus() {
    float x = 2000.f;
    float y = static_cast<float>(std::rand() % 400 + 500);

    int type = std::rand() % 3;
    std::unique_ptr<Bonus> bonus;

    switch (type) {
    case 0:
        bonus = std::make_unique<SlowModeBonus>(slowBonusTexture, sf::Vector2f(x, y), 5.f);
        break;
    case 1:
        bonus = std::make_unique<InvincibilityBonus>(invincibilityBonusTexture, sf::Vector2f(x, y), 5.f);
        break;
    case 2:
        bonus = std::make_unique<ScoreMultiplierBonus>(scoreBonusTexture, sf::Vector2f(x, y), 5.f);
        break;
    }

    activeBonuses.push_back(std::move(bonus));
}