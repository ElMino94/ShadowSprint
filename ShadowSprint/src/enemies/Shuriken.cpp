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

    float startX = static_cast<float>(std::rand() % 1700 + 100);
    float startY = -100.f;
    shape.setPosition(sf::Vector2f(startX, startY));

    sf::Vector2f dir = targetPos - shape.getPosition();
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    direction = (length > 0.f) ? dir / length : sf::Vector2f(0.f, 1.f);

    std::cout << " Shuriken spawn X=" << startX
        << " Y=" << startY
        << " Target=(" << targetPos.x << ", " << targetPos.y << ")\n";
}

void Shuriken::update(float dt) {
    shape.move(direction * speed * dt);
    shape.rotate(sf::degrees(rotationSpeed * dt));
}

void Shuriken::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Shuriken::getBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Shuriken::getPosition() const {
    return shape.getPosition();
}

bool Shuriken::isOffScreen() const {
    sf::Vector2f pos = shape.getPosition();
    return (pos.y > 1200.f || pos.x < -200.f || pos.x > 2100.f);
}
