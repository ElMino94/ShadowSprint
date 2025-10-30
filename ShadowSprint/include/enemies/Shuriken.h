#pragma once
#include <SFML/Graphics.hpp>
#include "../core/Entity.h"

class Shuriken : public Entity {
public:
    Shuriken(const sf::Vector2f& targetPos);

    void update(float dt);
    void draw(sf::RenderWindow& window) override;

    void move(const sf::Vector2f& offset) override;
    void setPosition(const sf::Vector2f& pos) override;
    sf::Vector2f getPosition() const override;
    sf::FloatRect getBounds() const override;

    bool isOffScreen() const;

private:
    sf::Vector2f direction;
    float baseSpeed;
    float speed;
    float rotationSpeed;
    sf::CircleShape shape;
};
