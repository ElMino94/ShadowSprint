#pragma once
#include <SFML/Graphics.hpp>


using namespace sf;

class Game {
public:
    Game();
    void run();

    enum GameState { MAINMENU, NORMALGAME, WTFGAME };

    GameState currentState;

private:
    RenderWindow window;
    Clock clock;

    void processEvents();
    void update(float dt);
    void render();
};