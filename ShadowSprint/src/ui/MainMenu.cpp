#include "../../include/ui/MainMenu.h"

MainMenu::MainMenu(RenderWindow& windowRef) : window(windowRef), font("../assets/fonts/samurai-blast.ttf"),
titleText(font), playText(font), optionText(font), quitText(font)
{
    titleText.setString("Ninja Runner");
    titleText.setCharacterSize(250);
    titleText.setFillColor(Color::White);

    playText.setString("Jouer");
    playText.setCharacterSize(100);
    playText.setFillColor(Color::White);

    optionText.setString("Options");
    optionText.setCharacterSize(100);
    optionText.setFillColor(Color::White);

    quitText.setString("Quitter");
    quitText.setCharacterSize(100);
    quitText.setFillColor(Color::White);

    Vector2u windowSize = window.getSize();

    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.getCenter());
    titleText.setPosition({ windowSize.x / 2.f, 200.f });

    FloatRect playBounds = playText.getLocalBounds();
    playText.setOrigin(playBounds.getCenter());
    playText.setPosition({ windowSize.x / 2.f, 500.f });

    FloatRect optionsBounds = optionText.getLocalBounds();
    optionText.setOrigin(optionsBounds.getCenter());
    optionText.setPosition({ windowSize.x / 2.f, 650.f });

    FloatRect quitBounds = quitText.getLocalBounds();
    quitText.setOrigin(quitBounds.getCenter());
    quitText.setPosition({ windowSize.x / 2.f, 800.f });
}

void MainMenu::update(float dt) {}

MainMenu::MainMenuAction MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (firstActivation && activationClock.getElapsedTime() < activationDelay) {
        return MainMenuAction::None;
    }

    firstActivation = false;

    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (playText.getGlobalBounds().contains(mousePos)) {
                return MainMenuAction::Play;
            }
            if (optionText.getGlobalBounds().contains(mousePos)) {
                return MainMenuAction::Options;
            }
            if (quitText.getGlobalBounds().contains(mousePos)) {
                return MainMenuAction::Quit;
            }
        }
    }

    return MainMenuAction::None;
}


void MainMenu::draw(RenderWindow& window) const {
    window.draw(titleText);
    window.draw(playText);
    window.draw(optionText);
    window.draw(quitText);
}

void MainMenu::activate() {
    activationClock.restart();
    firstActivation = true;
}

void MainMenu::setFullscreen(bool enabled) {
    unsigned int titleSize = enabled ? 250 : 150;
    unsigned int itemSize = enabled ? 100 : 60;

    titleText.setCharacterSize(titleSize);
    playText.setCharacterSize(itemSize);
    optionText.setCharacterSize(itemSize);
    quitText.setCharacterSize(itemSize);

    Vector2u windowSize = window.getSize();

    titleText.setOrigin(titleText.getLocalBounds().getCenter());
    titleText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.15f });

    playText.setOrigin(playText.getLocalBounds().getCenter());
    playText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.4f });

    optionText.setOrigin(optionText.getLocalBounds().getCenter());
    optionText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.55f });

    quitText.setOrigin(quitText.getLocalBounds().getCenter());
    quitText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.7f });
}