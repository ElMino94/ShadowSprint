#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../../include/ui/Utils.h"

class ParallaxLayer {
public:
    void init(const sf::Vector2f& size, const std::string& texturePath, float factor, float y);
    void update(float cameraX, const sf::Vector2f& size);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape stripA, stripB;
    float speedFactor = 0.f;
    std::shared_ptr<sf::Texture> texture;
};

class Map {
public:
    explicit Map(const sf::Vector2u& screenSize);
    void reset();
    float update(float dt);
    void draw(sf::RenderWindow& window);

    // Collision
    bool resolvePlayerCollisions(sf::FloatRect& playerBounds, sf::Vector2f& velocity);

    // (Pickups éventuels)
    int tryConsumePickup(const sf::FloatRect& playerBounds);

    // Pour placer le joueur sur le sol
    float getGroundY() const;

private:
    ParallaxLayer makeLayer(const std::string& file, float speedFactor, float y);

    sf::Vector2u screen;
    std::vector<ParallaxLayer> layers;
    std::vector<sf::RectangleShape> tiles;
    std::unique_ptr<sf::Texture> groundTexture;
    sf::Vector2f tileSize;

    float groundY = 0.f;
    float cameraX = 0.f;
    float baseSpeed = 0.f;
    float speed = 0.f;
    float totalMeters = 0.f;
    float lastSpeedStepMeters = 0.f;
    Utils::RNG rng;
    const float metersPerPixel = 0.1f;
};
