#include "../../include/bonus/Bonus.h"
#include <cmath>

Bonus::Bonus(const sf::Texture& texture, const sf::Vector2f& position, float duration) : duration(duration)
{
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setPosition(position);

    sf::Vector2u texSize = texture.getSize();
    sprite->setOrigin(sf::Vector2f(texSize.x / 2.f, texSize.y / 2.f));
    sprite->setScale(sf::Vector2f(0.4f, 0.4f));
}

void Bonus::update(float dt, float mapSpeed) {
    timer += dt;

    sprite->move(sf::Vector2f(-mapSpeed * 0.5f * dt, 0.f));

    sprite->move(velocity * dt);

    sprite->move(sf::Vector2f(0.f, std::sin(timer * 3.f) * 0.5f));
}

void Bonus::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}

bool Bonus::isExpired() const {
    return timer >= duration;
}

sf::FloatRect Bonus::getBounds() const {
    return sprite->getGlobalBounds();
}

sf::Vector2f Bonus::getCenter() const {
    auto bounds = sprite->getGlobalBounds();
    return bounds.position + bounds.size / 2.f;
}

bool Bonus::isOffScreen() const {
    return sprite->getGlobalBounds().getCenter().x < 0.f;
}