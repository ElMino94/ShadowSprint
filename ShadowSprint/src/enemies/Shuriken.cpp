#include "../../include/enemies/Shuriken.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

Shuriken::Shuriken(const sf::Vector2f& targetPos)
	: speed(800.f)

{
	if (!texture.loadFromFile("../assets/textures/Suriken/04.png")) {     
		std::cerr << " Impossible de charger la texture \n";

		sf::Image img;
		img.resize({ 32, 32 });
		sf::Image img;
		img.resize({ 32u, 32u });

		for (unsigned int y = 0; y < 32; ++y) {
			for (unsigned int x = 0; x < 32; ++x) {
				img.setPixel({ x, y }, sf::Color::Red);
			}
		}
		texture.loadFromImage(img);
	}

	sprite.setTexture(texture);
	
	sf::Vector2u texSize = texture.getSize();
	sprite.setOrigin(sf::Vector2f(texSize.x / 2.f, texSize.y / 2.f));
	sprite.setScale(sf::Vector2f(1.f, 1.f));

	float startX = static_cast<float>(std::rand() % 1700 + 100);
	float startY = -60.f;
	sprite.setPosition(sf::Vector2f(startX, startY));

	sf::Vector2f dir = targetPos - sprite.getPosition();
	float lenght = std::sqrt(dir.x * dir.x + dir.y * dir.y);

	if (lenght < 0.f)
		direction = dir / lenght;
	else
		direction = sf::Vector2f(0.f, 1.f);
}

void Shuriken::update(float dt) {

	move(direction * speed * dt);
	sprite.rotate(sf::degrees(720.f * dt));
}

bool Shuriken::isOffScreen() const {

	sf::Vector2f pos = sprite.getPosition();
	return (pos.y > 1200.f || pos.x < -200.f || pos.x > 2100.f);

}
