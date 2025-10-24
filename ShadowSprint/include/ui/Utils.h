#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace Utils {

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

    inline bool intersectsAABB(const sf::FloatRect& a, const sf::FloatRect& b) {
        return (a.position.x < b.position.x + b.size.x) &&
            (a.position.x + a.size.x > b.position.x) &&
            (a.position.y < b.position.y + b.size.y) &&
            (a.position.y + a.size.y > b.position.y);
    }

    struct RNG {
        RNG() {
            static bool seeded = false;
            if (!seeded) { std::srand(static_cast<unsigned>(std::time(nullptr))); seeded = true; }
        }
        float frand(float a, float b) {
            return a + (b - a) * (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
        }
        int irand(int a, int b) {
            if (b <= a) return a;
            return a + (std::rand() % (b - a + 1));
        }
    };

    inline bool resolveAABB(const sf::FloatRect& a, const sf::FloatRect& b, sf::Vector2f& push) {
        bool overl = intersectsAABB(a, b);
        if (!overl) { push = { 0.f, 0.f }; return false; }

        float aRight = a.position.x + a.size.x;
        float bRight = b.position.x + b.size.x;
        float aBottom = a.position.y + a.size.y;
        float bBottom = b.position.y + b.size.y;

        float dx1 = aRight - b.position.x;   
        float dx2 = bRight - a.position.x;   
        float dy1 = aBottom - b.position.y;  
        float dy2 = bBottom - a.position.y;  

        float ox = std::min(dx1, dx2);
        float oy = std::min(dy1, dy2);

        if (ox < oy) {
            push = { (dx1 < dx2) ? -ox : ox, 0.f };
        }
        else {
            push = { 0.f, (dy1 < dy2) ? -oy : oy };
        }
        return true;
    }
}