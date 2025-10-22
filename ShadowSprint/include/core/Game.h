#pragma once
#include <SFML/Graphics.hpp>

#include "../ui/MainMenu.h"
#include "../ui/OptionMenu.h"
#include "../ui/InGameUI.h"
#include "../ui/PauseMenu.h"

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

    void processEvents();
    void update(float dt);
    void render();

    void applyDisplaySettings(bool fullscreen, bool vsync);
    void resetGame();
};