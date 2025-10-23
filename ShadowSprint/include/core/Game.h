#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../player/Player.h"
#include "../ui/MainMenu.h"
#include "../ui/OptionMenu.h"
#include "../ui/InGameUI.h"
#include "../ui/PauseMenu.h"
#include "../ui/optionMenu.h"
#include "../enemies/Shuriken.h"

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
    bool gameStarted;
    bool gameOver;
    float countdown;

    State currentState;


    void processEvents();
    void update(float dt);
    void render();
    void resetGame();
};

    void applyDisplaySettings(bool fullscreen, bool vsync);
    void resetGame();
};
