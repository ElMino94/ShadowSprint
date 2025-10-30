#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Platform {
public:
    Platform(const sf::Vector2f& position, int tileCount, const sf::Texture& texture, const sf::Vector2f& tileSize);

    void update(float dt, float mapSpeed);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    bool isOffScreen() const;

private:
    std::vector<sf::Sprite> tiles;
    sf::Vector2f tileSize;
};
