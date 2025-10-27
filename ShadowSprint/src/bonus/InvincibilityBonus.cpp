#include "../../include/bonus/InvincibilityBonus.h"
#include "../../include/player/Player.h"

InvincibilityBonus::InvincibilityBonus(const sf::Texture& texture, const sf::Vector2f& position, float duration) : Bonus(texture, position, duration) {}

void InvincibilityBonus::apply(Player& player) {
    player.activateInvincibility(duration);
}