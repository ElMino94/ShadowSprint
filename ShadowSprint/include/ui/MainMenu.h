#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class MainMenu {
public:
    MainMenu(RenderWindow& window);
    void update(float dt);
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(RenderWindow& window) const;

private:
    RenderWindow& window;

    Font font;
    Text titleText;
    Text playText;
    Text quitText;

    RectangleShape playButton;
    RectangleShape quitButton;
};