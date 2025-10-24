#pragma once
#include "Bonus.h"

class InvincibilityBonus : public Bonus {
public:
    InvincibilityBonus(const sf::Texture& texture, const sf::Vector2f& position, float duration);
    void apply(Player& player) override;
};