#include "../../include/ui/MainMenu.h"
#include "../../include/ui/UIUtils.h"

using namespace sf;
using namespace UIUtils;

MainMenu::MainMenu(RenderWindow& windowRef) : window(windowRef), font("../assets/fonts/samurai-blast.ttf"),
titleText(font), playText(font), optionText(font), quitText(font)
{
    titleText.setString("Ninja Runner");
    playText.setString("Jouer");
    optionText.setString("Options");
    quitText.setString("Quitter");

    styleText(titleText, 250);
    styleText(playText, 100);
    styleText(optionText, 100);
    styleText(quitText, 100);

    positionText(titleText, window, 0.15f);
    positionText(playText, window, 0.4f);
    positionText(optionText, window, 0.55f);
    positionText(quitText, window, 0.7f);
}

void MainMenu::update(float dt) {}

MainMenu::MainMenuAction MainMenu::handleEvent(const sf::Event& event) {
    if (firstActivation && activationClock.getElapsedTime() < activationDelay) {
        return MainMenuAction::None;
    }

    firstActivation = false;

    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (playText.getGlobalBounds().contains(mousePos)) return MainMenuAction::Play;
            if (optionText.getGlobalBounds().contains(mousePos)) return MainMenuAction::Options;
            if (quitText.getGlobalBounds().contains(mousePos)) return MainMenuAction::Quit;
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

    styleText(titleText, titleSize);
    styleText(playText, itemSize);
    styleText(optionText, itemSize);
    styleText(quitText, itemSize);

    positionText(titleText, window, 0.15f);
    positionText(playText, window, 0.4f);
    positionText(optionText, window, 0.55f);
    positionText(quitText, window, 0.7f);
}