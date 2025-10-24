#pragma once
#include "Bonus.h"

class SlowModeBonus : public Bonus {
public:
    SlowModeBonus(const sf::Texture& texture, const sf::Vector2f& position, float duration);
    void apply(Player& player) override;
};