#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>
#include <vector>
#include "../ui/Utils.h"

struct ParallaxLayer {
    sf::RectangleShape stripA;
    sf::RectangleShape stripB;
    float speedFactor = 1.f;
    std::shared_ptr<sf::Texture> texture;

    void init(const sf::Vector2f& size, const std::string& texturePath, float factor, float y);
    void update(float cameraX, const sf::Vector2f& size);
    void draw(sf::RenderWindow& window);
};

class Map {
public:
    explicit Map(const sf::Vector2u& screenSize);

    float update(float dt);
    void draw(sf::RenderWindow& window);

    bool resolvePlayerCollisions(sf::FloatRect& playerBounds, sf::Vector2f& velocity);
    int tryConsumePickup(const sf::FloatRect& playerBounds);

private:
    ParallaxLayer makeLayer(const std::string& file, float speedFactor, float y);
    void reset();

    sf::Vector2u screen;
    Utils::RNG rng;

    float cameraX = 0.f;
    float speed = 220.f;
    float baseSpeed = 220.f;
    float metersPerPixel = 1.f / 50.f;
    float totalMeters = 0.f;
    float lastSpeedStepMeters = 0.f;

    std::unique_ptr<sf::Texture> groundTexture;
    std::vector<sf::RectangleShape> tiles;
    sf::Vector2f tileSize{ 40.f, 40.f };
    float groundY = 900.f;

    std::vector<ParallaxLayer> layers;
};
