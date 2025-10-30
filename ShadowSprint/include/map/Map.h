#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../../include/ui/Utils.h"
#include "../../include/map/Platform.h"

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
    float update(float dt, float score);
    void draw(sf::RenderWindow& window);

    int tryConsumePickup(const sf::FloatRect& playerBounds);

    float getGroundY() const;

    const std::vector <std::unique_ptr<class Platform>>& getPlatforms() const;

private:
    ParallaxLayer makeLayer(const std::string& file, float speedFactor, float y);

    sf::Vector2u screen;
    float cameraX = 0.f;
    float baseSpeed = 220.f;
    float speed = 0.f;
    float lastSpeedStepMeters = 0.f;
    float groundY = 0.f;
    float metersPerPixel = 0.05f;

    std::unique_ptr<sf::Texture> groundTexture;
    std::vector<sf::RectangleShape> tiles;
    sf::Vector2f tileSize;

    std::vector<ParallaxLayer> layers;

    std::vector<std::unique_ptr<class Platform>> platforms;
    float platformSpawnTimer = 0.f;

    Utils::RNG rng;
};
