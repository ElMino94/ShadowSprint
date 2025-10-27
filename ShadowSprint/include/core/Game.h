#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "../map/Map.h"
#include "../player/Player.h"
#include "../ui/MainMenu.h"
#include "../ui/OptionMenu.h"
#include "../ui/PauseMenu.h"
#include "../ui/InGameUI.h"
#include "../enemies/Shuriken.h"
#include "../ui/Utils.h"
#include "../bonus/InvincibilityBonus.h"
#include "../bonus/ScoreMultiplierBonus.h"
#include "../bonus/SlowModeBonus.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void applyDisplaySettings(bool fullscreen, bool vsync);
    void resetGame();

    sf::RenderWindow window;
    sf::Clock clock;

    enum State { MAINMENU, OPTIONSMENU, PAUSEMENU, PLAYING } currentState;

    MainMenu mainMenu;
    OptionMenu optionMenu;
    PauseMenu pauseMenu;
    InGameUI igUI;

    std::unique_ptr<Map> map;
    Player player;

    sf::Font font;
    sf::Text countdownText;
    sf::Text gameOverText;

    sf::Clock shurikenClock;

    sf::Clock bonusSpawnClock;
    float bonusSpawnInterval = 2.f;

    sf::Texture slowBonusTexture;
    sf::Texture invincibilityBonusTexture;
    sf::Texture scoreBonusTexture;

    std::vector<std::unique_ptr<Bonus>> activeBonuses;

    bool gameStarted;
    bool gameOver;
    float countdown;
    float playerSpeed;
    float score = 0.f;
    float playerSpeed = 1.f;

    // --- Gestion des Shurikens ---
    std::vector<std::unique_ptr<Shuriken>> shurikens;
    float shurikenSpawnTimer = 0.f;

    void processEvents();
    void update(float dt);
    void render();
    void resetGame();

    void applyDisplaySettings(bool fullscreen, bool vsync);

    void spawnRandomBonus();
};
