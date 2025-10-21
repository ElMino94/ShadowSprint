#pragma once
#include "SFML/Graphics.hpp"
#include "../core/Entity.h"

class Shuriken : public Entity{
public:
	Shuriken(const sf::Vector2f& targetPos);

	void update(float dt) override;
	bool isOffScreen() const;

private:

	sf::Vector2f direction;
	float speed;
};

