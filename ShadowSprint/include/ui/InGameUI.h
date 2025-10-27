#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

class InGameUI {
public:
    InGameUI(sf::RenderWindow& window);

    void update(float dt, float distanceMeters);
    void draw(sf::RenderWindow& window) const;

    void setFullscreen(bool enabled);

    void addBonusIcon(const sf::Texture& texture, float duration);
    void updateBonusIcons(float dt);

private:
    sf::RenderWindow& window;
    sf::Font font;

    sf::Text distanceText;

    sf::Texture scoreBackgroundTexture;
    sf::Sprite scoreBackgroundSprite;

    unsigned int baseSize = 40;

    struct ActiveBonusIcon {
        sf::Sprite sprite;
        float remainingTime;

        ActiveBonusIcon(const sf::Texture& texture, float duration)
            : sprite(texture), remainingTime(duration) {
            sprite.setScale({ 0.4f, 0.4f });
        }
    };

    std::vector<ActiveBonusIcon> activeBonusIcons;
};