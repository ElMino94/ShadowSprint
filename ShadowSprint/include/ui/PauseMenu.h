#pragma once
#include <SFML/Graphics.hpp>

class PauseMenu {
public:
    enum class Action {
        None,
        Resume,
        Options,
        Restart,
        BackToMenu
    };

    PauseMenu(sf::RenderWindow& window);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    Action handleEvent(const sf::Event& event);

    void activate();

private:
    sf::RenderWindow& window;
    sf::Font font;

    sf::Text titleText;
    sf::Text resumeText;
    sf::Text optionsText;
    sf::Text restartText;
    sf::Text backText;

    bool firstActivation = false;
    sf::Clock activationClock;
    sf::Time activationDelay = sf::seconds(0.2f);
};