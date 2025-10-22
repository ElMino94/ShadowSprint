#pragma once
#include <SFML/Graphics.hpp>

#include "../ui/MainMenu.h"
#include "../ui/OptionMenu.h"
#include "../ui/InGameUI.h"

using namespace sf;

class Game {
public:
    Game();
    void run();

    enum GameState { PLAYING, MAINMENU, OPTIONSMENU, PAUSEMENU };

    GameState currentState;

private:
    RenderWindow window;
    Clock clock;

    MainMenu mainMenu;
    OptionMenu optionMenu;
    InGameUI igUI;

    void processEvents();
    void update(float dt);
    void render();
};