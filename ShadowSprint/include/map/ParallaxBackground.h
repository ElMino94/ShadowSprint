#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class ParallaxBackground {

public:

	ParallaxBackground() = default;
	void addLayer(const sf::Texture& texture, float speed, float y = 0.f);
	void setTrainLayer(const sf::Texture& texture, float speed, float y, float delay);
	void update(float dt);
	void draw(sf::RenderWindow& window);

private:

};