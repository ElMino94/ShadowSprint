#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../player/Player.h"
#include "../ui/MainMenu.h"
#include "../ui/optionMenu.h"
#include "../enemies/Shuriken.h"

class Game {
public:
    Game();
    void run();

private:
    enum State {
        MAINMENU,
        OPTIONSMENU,
        PLAYING
    };

    sf::RenderWindow window;
    MainMenu mainMenu;
    OptionMenu optionMenu;
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
