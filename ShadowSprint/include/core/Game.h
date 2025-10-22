#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "../ui/MainMenu.h"
#include "../ui/OptionMenu.h"
#include "../player/Player.h"
#include "../enemies/Shuriken.h"

using namespace sf;

class Game {
public:
    Game();
    void run();

    enum GameState { PLAYING, MAINMENU, OPTIONSMENU, PAUSEMENU };

private:
    RenderWindow window;
    Clock clock;

    MainMenu mainMenu;
    OptionMenu optionMenu;

    Player player;
    std::vector<Shuriken> shurikens;
    Clock ShurikenClock;

    bool gameStarted;
    bool gameOver;
    float countdown;
    GameState currentState;

    Font font;
    Text countdownText;
    Text gameOverText;

    void processEvents();
    void update(float dt);
    void render();

    void resetGame();
};