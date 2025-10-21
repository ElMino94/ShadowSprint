#pragma once
#include <SFML/Graphics.hpp>

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

    GameState currentState;

private:
    RenderWindow window;
    Clock clock;

    MainMenu mainMenu;
    OptionMenu optionMenu;

    void processEvents();
    void update(float dt);
    void render();
};