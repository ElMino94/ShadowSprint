#include "../../include/bonus/ScoreMultiplierBonus.h"
#include "../../include/player/Player.h"

ScoreMultiplierBonus::ScoreMultiplierBonus(const sf::Texture& texture, const sf::Vector2f& position, float duration) : Bonus(texture, position, duration) {}

void ScoreMultiplierBonus::apply(Player& player) {
    player.activateScoreMultiplier(2.f, duration);
}