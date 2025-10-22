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

    inline void positionTextTopRight(sf::Text& text, const sf::RenderWindow& window, float offsetY = 20.f, float marginX = 30.f) {
        text.setOrigin(text.getLocalBounds().getCenter());
        sf::Vector2u size = window.getSize();
        text.setPosition({ size.x - marginX, offsetY });
    }

    inline void styleText(sf::Text& text, unsigned int size, const sf::Color& color = sf::Color::White) {
        text.setCharacterSize(size);
        text.setFillColor(color);
    }

    inline void applyHoverEffect(sf::Text& text, const sf::RenderWindow& window, const sf::Color& normalColor = sf::Color::White, const sf::Color& hoverColor = sf::Color::Yellow) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (text.getGlobalBounds().contains(mousePos)) {
            text.setFillColor(hoverColor);
        }
        else {
            text.setFillColor(normalColor);
        }
    }
}