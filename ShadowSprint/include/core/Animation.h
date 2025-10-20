#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animation {

private:

	sf::Texture* texture;
	std::vector<sf::IntRect> frames;
	int currentFrame;
	float frameTime;
	float elapsed;

public:

	Animation();
	Animation(sf::Texture& texture, int frameCount, float frameTime, sf::Vector2i frameSize);

	void update(float dt);
	void applyToSprite(sf::Sprite& sprite);
	void reset();
};