#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>
#include "../ui/Utils.h"

struct Platform {
	sf::RectangleShape shape;
	sf::FloatRect bounds() const { return shape.getGlobalBounds(); }
};


enum class PickupType { Bonus, Malus };


struct Pickup {
	PickupType type;
	sf::RectangleShape shape;
	sf::FloatRect bounds() const { return shape.getGlobalBounds(); }
};


struct ParallaxLayer {
	sf::RectangleShape stripA;
	sf::RectangleShape stripB;
	float speedFactor = 1.f; 


	void init(const sf::Vector2f& size, const sf::Color& color, float factor, float y);
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


	float getSpeed() const { return speed; }
	float getTotalMeters() const { return totalMeters; }
	float getGroundY() const { return groundY; }


private:
	void reset();
	void spawnChunk();


	sf::Vector2u screen;
	Utils::RNG rng;


	float cameraX = 0.f;
	float speed = 220.f;
	float baseSpeed = 220.f;
	float metersPerPixel = 1.f / 50.f; 
	float totalMeters = 0.f;
	float lastSpeedStepMeters = 0.f; 


	float speedBuffTimer = 0.f;
	float speedNerfTimer = 0.f;


	std::vector<sf::RectangleShape> tiles;
	sf::Vector2f tileSize{ 40.f, 40.f };
	float groundY = 900.f; 

	std::vector<ParallaxLayer> layers;
};