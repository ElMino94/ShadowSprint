#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

class InGameUI {
public:
    InGameUI(sf::RenderWindow& window);

    void update(float dt, float distanceMeters);
    void draw(sf::RenderWindow& window) const;

    void setFullscreen(bool enabled);

private:
    sf::RenderWindow& window;
    sf::Font font;

    sf::Text distanceText;

    sf::Texture scoreBackgroundTexture;
    sf::Sprite scoreBackgroundSprite;

    unsigned int baseSize = 40;
};