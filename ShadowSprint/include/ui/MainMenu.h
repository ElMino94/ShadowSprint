#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class MainMenu {
public:
    enum class MainMenuAction {
        None,
        Play,
        Options,
        Quit
    };

    MainMenu(RenderWindow& window);
    void update(float dt);
    MainMenuAction handleEvent(const Event& event, RenderWindow& window);
    void draw(RenderWindow& window) const;

    void activate();
    void setFullscreen(bool enabled);

private:
    RenderWindow& window;

    Font font;
    Text titleText;
    Text playText;
    Text optionText;
    Text quitText;

    Clock activationClock;
    bool firstActivation = true;
    const Time activationDelay = milliseconds(200);
};