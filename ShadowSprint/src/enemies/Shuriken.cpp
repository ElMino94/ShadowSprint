#include "../../include/enemies/Shuriken.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

Shuriken::Shuriken(const sf::Vector2f& targetPos)
	: speed(800.f)

{
	if (!texture.loadFromFile("../assets/shuriken.png")) {       //Refaire le chemin du png
		std::cerr << " Impossible de charger la texture \n";
	}

	sprite.setTexture(texture);
	sprite.setScale({ 0.15f, 0.15f });
	
	float startX = static_cast<float>(std::rand() % 1800 + 60);
	float startY = -50.f;
	sprite.setPosition({ startX, startY });

	sf::Vector2f dir = targetPos - sprite.getPosition();
	float lenght = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	if (lenght != 0.f)
		direction = dir / lenght;
	else
		direction = { 0.f, 1.f };
}

void Shuriken::update(float dt) {

	move(direction * speed * dt);

	sprite.rotate(sf::degrees(1000.f * dt));
}

bool Shuriken::isOffScreen() const {

	sf::Vector2f pos = sprite.getPosition();
	return (pos.y > 1200.f || pos.x < -200.f || pos.x > 2100.f);

}
