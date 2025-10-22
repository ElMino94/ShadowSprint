#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual void move(const sf::Vector2f& offset) = 0;
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual sf::FloatRect getBounds() const = 0;

protected:
    sf::Vector2f velocity{ 0.f, 0.f }; 
};
