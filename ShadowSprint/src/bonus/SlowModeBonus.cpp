#include "../../include/bonus/SlowModeBonus.h"
#include "../../include/player/Player.h"

SlowModeBonus::SlowModeBonus(const sf::Texture& texture, const sf::Vector2f& position, float duration) : Bonus(texture, position, duration) {}

void SlowModeBonus::apply(Player& player) {
    //player.setSlowMode(true);
}