#include "../../include/ui/MainMenu.h"

MainMenu::MainMenu(RenderWindow& windowRef) : window(windowRef),
font("../assets/fonts/Karasha.otf"),
titleText(font),
playText(font),
quitText(font)
{
    titleText.setString("Ninja Runner");
    titleText.setCharacterSize(300);
    titleText.setFillColor(sf::Color::White);

    playText.setString("Jouer");
    playText.setCharacterSize(100);
    playText.setFillColor(sf::Color::White);

    quitText.setString("Quitter");
    quitText.setCharacterSize(100);
    quitText.setFillColor(sf::Color::White);

    Vector2u windowSize = window.getSize();

    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.getCenter());
    titleText.setPosition({ windowSize.x / 2.f, 200.f });

    FloatRect playBounds = playText.getLocalBounds();
    playText.setOrigin(playBounds.getCenter());
    playText.setPosition({ windowSize.x / 2.f, 500.f });

    FloatRect quitBounds = quitText.getLocalBounds();
    quitText.setOrigin(quitBounds.getCenter());
    quitText.setPosition({ windowSize.x / 2.f, 800.f });
}

void MainMenu::update(float dt) {}

bool MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

            if (playText.getGlobalBounds().contains(mousePos)) {
                return true;
            }

            if (quitText.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }
    }
    return false;
}

void MainMenu::draw(sf::RenderWindow& window) const {
    window.draw(titleText);
    window.draw(playText);
    window.draw(quitText);
}