#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../player/Player.h"
#include "../ui/MainMenu.h"
#include "../ui/OptionMenu.h"
#include "../ui/InGameUI.h"
#include "../ui/PauseMenu.h"
#include "../enemies/Shuriken.h"
#include "../ui/Utils.h"
#include "../bonus/InvincibilityBonus.h"
#include "../bonus/ScoreMultiplierBonus.h"
#include "../bonus/SlowModeBonus.h"

class Game {
public:
    Game();
    void run();
    
    enum GameState { PLAYING, MAINMENU, OPTIONSMENU, PAUSEMENU };

    GameState currentState;

private:
    sf::RenderWindow window;
    sf::Clock clock;

    MainMenu mainMenu;
    OptionMenu optionMenu;
    PauseMenu pauseMenu;
    InGameUI igUI;
    
    Player player;

    std::vector<std::unique_ptr<Shuriken>> shurikens;

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

    void processEvents();
    void update(float dt);
    void render();
    void resetGame();

    void applyDisplaySettings(bool fullscreen, bool vsync);

    void spawnRandomBonus();
};