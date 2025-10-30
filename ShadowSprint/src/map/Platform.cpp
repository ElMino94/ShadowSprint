#include "../../include/map/Platform.h"
#include <SFML/Graphics/Sprite.hpp>

Platform::Platform(const sf::Vector2f& position, int tileCount, const sf::Texture& texture, const sf::Vector2f& tileSize)
    : tileSize(tileSize)
{
    tiles.reserve(static_cast<size_t>(tileCount));

    for (int i = 0; i < tileCount; ++i)
    {
        sf::Sprite tile(texture);
        tile.setTexture(texture);

        tile.setPosition(sf::Vector2f(position.x + i * tileSize.x, position.y));

        sf::IntRect texRect({ 0, 0 },
            { static_cast<int>(tileSize.x), static_cast<int>(tileSize.y) });
        tile.setTextureRect(texRect);

        tiles.push_back(tile);
    }
}

void Platform::update(float dt, float mapSpeed)
{
    const sf::Vector2f offset(-mapSpeed * dt, 0.f);
    for (auto& tile : tiles)
        tile.move(offset);
}

void Platform::draw(sf::RenderWindow& window) const
{
    for (const auto& tile : tiles)
        window.draw(tile);
}

sf::FloatRect Platform::getBounds() const
{
    if (tiles.empty())
        return sf::FloatRect({ 0.f, 0.f }, { 0.f, 0.f });

    const sf::Vector2f pos = tiles.front().getPosition();
    const float width = tileSize.x * static_cast<float>(tiles.size());

    return sf::FloatRect(pos, sf::Vector2f(width, tileSize.y));
}

bool Platform::isOffScreen() const
{
    if (tiles.empty())
        return true;

    const float rightEdge = tiles.back().getPosition().x + tileSize.x;
    return rightEdge < 0.f;
}
