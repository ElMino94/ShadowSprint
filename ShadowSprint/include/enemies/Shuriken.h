#pragma once
#include <SFML/Graphics.hpp>
#include "../core/Entity.h"

class Shuriken : public Entity {
public:
    Shuriken(const sf::Vector2f& targetPos);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    bool isOffScreen() const;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const override;

private:
    sf::Vector2f direction;
    float speed;
    float rotationSpeed;
    sf::CircleShape shape;
};
