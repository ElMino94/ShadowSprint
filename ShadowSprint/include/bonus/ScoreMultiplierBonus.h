#pragma once
#include "Bonus.h"

class ScoreMultiplierBonus : public Bonus {
public:
    ScoreMultiplierBonus(const sf::Texture& texture, const sf::Vector2f& position, float duration);
    void apply(Player& player) override;
};