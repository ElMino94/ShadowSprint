#include "../../include/enemies/Shuriken.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

Shuriken::Shuriken(const sf::Vector2f& targetPos)
    : speed(400.f), rotationSpeed(360.f)
{
    float radius = 15.f;
    shape = sf::CircleShape(radius, 6);
    shape.setFillColor(sf::Color(255, 220, 0));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.f);
    shape.setOrigin(sf::Vector2f(radius, radius));

    float startX = 2100.f; 
    float startY = static_cast<float>(std::rand() % 600 + 400); 
    shape.setPosition(sf::Vector2f(startX, startY));

    sf::Vector2f offset(0.f, static_cast<float>((std::rand() % 201) - 100)); 
    sf::Vector2f dir = (targetPos + offset) - shape.getPosition();
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    direction = (length > 0.f) ? dir / length : sf::Vector2f(-1.f, 0.f);
}

void Shuriken::update(float dt) {
    shape.move(direction * speed * dt);
    shape.rotate(sf::degrees(rotationSpeed * dt));
}

void Shuriken::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Shuriken::move(const sf::Vector2f& offset) { shape.move(offset); }
void Shuriken::setPosition(const sf::Vector2f& pos) { shape.setPosition(pos); }
sf::Vector2f Shuriken::getPosition() const { return shape.getPosition(); }
sf::FloatRect Shuriken::getBounds() const { return shape.getGlobalBounds(); }

bool Shuriken::isOffScreen() const {
    sf::Vector2f pos = shape.getPosition();
    return (pos.x < -200.f || pos.y < -200.f || pos.y > 1300.f);
}
