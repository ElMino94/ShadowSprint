#include "../../include/core/Entity.h"
#include <iostream>

Entity::Entity()
    : texture(), sprite(texture), velocity(0.f, 0.f) {}

void Entity::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Entity::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Entity::getPosition() const {
    return sprite.getPosition();
}

void Entity::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}

void Entity::move(const sf::Vector2f& offset) {
    sprite.move(offset);
}
