#include "../../include/core/Game.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace sf;
using namespace Utils;

Game::Game() : window(VideoMode({ 1920u, 1080u }), "ShadowSprint", Style::Default),
currentState(MAINMENU),
mainMenu(window), optionMenu(window), pauseMenu(window), igUI(window),
player(3.f),
font("../assets/fonts/samurai-blast.ttf"),
countdownText(font, "3", 200),
gameOverText(font, "GAME OVER", 150),
gameStarted(false),
gameOver(false),
countdown(3.f),
playerSpeed(1.f),
shurikenSpawnTimer(0.f)
{
    window.setFramerateLimit(60);

    countdownText.setFillColor(Color::White);
    countdownText.setPosition(Vector2f(900.f, 300.f));

    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(Vector2f(600.f, 400.f));

    map = std::make_unique<Map>(window.getSize());

    if (!slowBonusTexture.loadFromFile("../assets/textures/bonus_slowmode.png"))
        std::cerr << "Erreur chargement bonus_slowmode.png\n";
    if (!invincibilityBonusTexture.loadFromFile("../assets/textures/bonus_invincibility.png"))
        std::cerr << "Erreur chargement bonus_invincibility.png\n";
    if (!scoreBonusTexture.loadFromFile("../assets/textures/bonus_x2.png"))
        std::cerr << "Erreur chargement bonus_x2.png\n";
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

        // --- Menus ---
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
            default: break;
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
            default: break;
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
        float mapSpeed = map ? map->update(dt, score) : 0.f;

        // === Gestion des bonus ===
        if (bonusSpawnClock.getElapsedTime().asSeconds() > bonusSpawnInterval) {
            bonusSpawnClock.restart();
            spawnRandomBonus();
        }

        for (auto it = activeBonuses.begin(); it != activeBonuses.end();) {
            (*it)->update(dt, mapSpeed);

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

        // === Compte à rebours avant départ ===
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

        // === Gestion des shurikens ===
        else if (!gameOver) {
            // Apparition selon le score (paliers de 100 pts)
            float spawnDelay = 1.5f;
            if (shurikenClock.getElapsedTime().asSeconds() > spawnDelay) {
                shurikenClock.restart();

                int baseCount = 1;
                int extra = static_cast<int>(score / 100.f); // +1 shuriken tous les 100 pts
                int shurikenCount = std::min(baseCount + extra, 10); // max 10

                auto bounds = player.getBounds();
                Vector2f target = bounds.position + (bounds.size * 0.5f);

                for (int i = 0; i < shurikenCount; ++i) {
                    float offsetY = static_cast<float>((std::rand() % 400) - 200);
                    Vector2f targetOffset = target + Vector2f(0.f, offsetY);
                    shurikens.push_back(std::make_unique<Shuriken>(targetOffset));
                }
            }

            for (auto it = shurikens.begin(); it != shurikens.end();) {
                float shurikenSpeedFactor = player.isSlowMode() ? 0.3f : 1.f;
                (*it)->update(dt * shurikenSpeedFactor);

                FloatRect playerBounds = player.getBounds();
                FloatRect shurikenBounds = (*it)->getBounds();

                if (intersectsAABB(playerBounds, shurikenBounds)) {
                    if (player.isBlocking() || player.isInvincible()) {
                        it = shurikens.erase(it);
                        continue;
                    }
                    else {
                        gameOver = true;
                        break;
                    }
                }

                if ((*it)->isOffScreen())
                    it = shurikens.erase(it);
                else
                    ++it;
            }
        }

        // === Score et joueur ===
        if (!gameOver && gameStarted) {
            player.handleInput();
            score += 10.f * dt * player.getScoreMultiplier();
        }

        player.update(dt);
        player.updateSpeed(dt, score);
        player.updateBonusTimer(dt);

        sf::Vector2f vel(0.f, player.getVelocityY());

        constexpr float playerFootY = 1210.f;
        bool grounded = player.getPosition().y >= playerFootY;

        if (grounded && player.getVelocityY() >= 0.f) {
            vel.y = 0.f;
            player.setOnGround(true);
        }
        else {
            player.setOnGround(false);
        }

        player.move(vel * dt);

        if (player.getPosition().y > window.getSize().y + 200.f) {
            gameOver = true;
            std::cout << " Player fell off the map!" << std::endl;
        }

        // Nettoyage shurikens
        for (auto& s : shurikens)
            s->update(dt);

        shurikens.erase(
            std::remove_if(shurikens.begin(), shurikens.end(),
                [](const std::unique_ptr<Shuriken>& s) { return s->isOffScreen(); }),
            shurikens.end());

        int hit = map->tryConsumePickup(player.getBounds());
        if (hit > 0) score += 100.f;
        else if (hit < 0) score = std::max(0.f, score - 50.f);

        playerSpeed = std::max(1.f, mapSpeed / 200.f);
    }

                igUI.setBlockCooldown(player.getBlockCooldown(), player.getBlockCooldownDuration());
                igUI.update(dt, score);
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

    case PAUSEMENU:
        pauseMenu.draw(window);
        break;

    case PLAYING: {
        if (map) map->draw(window);
        player.draw(window);

        for (auto& s : shurikens)
            s->draw(window);

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
    VideoMode mode = fullscreen ? VideoMode::getDesktopMode() : VideoMode({ 1920, 1080u });
    unsigned int style = fullscreen ? Style::None : Style::Default;

    window.create(mode, "ShadowSprint", style);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(vsync);
}

void Game::resetGame() {
    gameStarted = false;
    gameOver = false;
    countdown = 3.f;
    score = 0.f;
    player.reset();
    shurikens.clear();
    shurikenSpawnTimer = 0.f;
    map = std::make_unique<Map>(window.getSize());
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
