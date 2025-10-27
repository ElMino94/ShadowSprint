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

    enum GameState { MAINMENU, OPTIONSMENU, PAUSEMENU, PLAYING };

    GameState currentState;

private:
    void processEvents();
    void update(float dt);
    void render();
    void applyDisplaySettings(bool fullscreen, bool vsync);
    void resetGame();

    sf::RenderWindow window;
    sf::Clock clock;

    MainMenu mainMenu;
    OptionMenu optionMenu;
    PauseMenu pauseMenu;
    InGameUI igUI;
    std::unique_ptr<Map> map;
    Player player;

    sf::Font font;
    sf::Text countdownText;
    sf::Text gameOverText;

    bool gameStarted;
    bool gameOver;
    float countdown;
    float playerSpeed;
    float score = 0.f;

    // Bonus
    sf::Texture slowBonusTexture;
    sf::Texture invincibilityBonusTexture;
    sf::Texture scoreBonusTexture;

    sf::Clock bonusSpawnClock;

    float bonusSpawnInterval = 2.f;

    void spawnRandomBonus();

    std::vector<std::unique_ptr<Bonus>> activeBonuses;

    // Shuriken
    sf::Clock shurikenClock;

    std::vector<std::unique_ptr<Shuriken>> shurikens;

    float shurikenSpawnTimer;
};
