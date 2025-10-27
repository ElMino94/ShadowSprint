#pragma once
#include <SFML/Graphics.hpp>

class Player;

class Bonus {
public:
    Bonus(const sf::Texture& texture, const sf::Vector2f& position, float duration);
    virtual ~Bonus() = default;

    virtual void apply(Player& player) = 0;

    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isExpired() const;
    bool isOffScreen() const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getCenter() const;

protected:
    std::unique_ptr<sf::Sprite> sprite;
    float timer = 0.f;
    float duration;
    sf::Vector2f velocity{ -200.f, 0.f };
};