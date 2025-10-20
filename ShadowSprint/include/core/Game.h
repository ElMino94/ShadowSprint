#pragma once
#include <SFML/Graphics.hpp>

#include "../ui/MainMenu.h"

using namespace sf;

class Game {
public:
    Game();
    void run();

    enum GameState { PLAYING, MAINMENU, PAUSEMENU };

    GameState currentState;

private:
    RenderWindow window;
    Clock clock;

    MainMenu mainMenu;

    void processEvents();
    void update(float dt);
    void render();
};