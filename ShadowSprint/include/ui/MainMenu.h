#pragma once
#include <SFML/Graphics.hpp>

class MainMenu {
public:
    enum class MainMenuAction {
        None,
        Play,
        Options,
        Quit
    };

    MainMenu(sf::RenderWindow& window);
    void update(float dt);
    MainMenuAction handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;

    void activate();
    void setFullscreen(bool enabled);

private:
    sf::RenderWindow& window;

    sf::Font font;
    sf::Text titleText;
    sf::Text playText;
    sf::Text optionText;
    sf::Text quitText;

    sf::Clock activationClock;
    bool firstActivation = true;
    const sf::Time activationDelay = sf::milliseconds(200);
};