#pragma once
#include <SFML/Graphics.hpp>

namespace UIUtils {

    inline void centerText(sf::Text& text) {
        text.setOrigin(text.getLocalBounds().getCenter());
    }

    inline void positionText(sf::Text& text, const sf::RenderWindow& window, float yRatio) {
        centerText(text);
        sf::Vector2u size = window.getSize();
        text.setPosition({ size.x / 2.f, size.y * yRatio });
    }

    inline void styleText(sf::Text& text, unsigned int size, const sf::Color& color = sf::Color::White) {
        text.setCharacterSize(size);
        text.setFillColor(color);
    }

}