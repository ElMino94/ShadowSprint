#include "../../include/ui/PauseMenu.h"
#include "../../include/ui/UIUtils.h"

using namespace sf;
using namespace UIUtils;

PauseMenu::PauseMenu(RenderWindow& windowRef) : window(windowRef), font("../assets/fonts/samurai-blast.ttf"),
titleText(font), resumeText(font), optionsText(font), restartText(font), backText(font)
{
    titleText.setString("Pause");
    resumeText.setString("Continuer");
    optionsText.setString("Options");
    restartText.setString("Recommencer");
    backText.setString("Menu principal");

    unsigned int titleSize = 120;
    unsigned int itemSize = 70;

    styleText(titleText, titleSize);
    styleText(resumeText, itemSize);
    styleText(optionsText, itemSize);
    styleText(restartText, itemSize);
    styleText(backText, itemSize);

    positionText(titleText, window, 0.20f);
    positionText(resumeText, window, 0.40f);
    positionText(optionsText, window, 0.50f);
    positionText(restartText, window, 0.60f);
    positionText(backText, window, 0.70f);
}

void PauseMenu::update(float dt) {
    applyHoverEffect(resumeText, window);
    applyHoverEffect(optionsText, window);
    applyHoverEffect(restartText, window);
    applyHoverEffect(backText, window);
}

PauseMenu::Action PauseMenu::handleEvent(const Event& event) {
    if (firstActivation && activationClock.getElapsedTime() < activationDelay) {
        return Action::None;
    }
    firstActivation = false;

    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

            if (resumeText.getGlobalBounds().contains(mousePos)) return Action::Resume;
            if (optionsText.getGlobalBounds().contains(mousePos)) return Action::Options;
            if (restartText.getGlobalBounds().contains(mousePos)) return Action::Restart;
            if (backText.getGlobalBounds().contains(mousePos)) return Action::BackToMenu;
        }
    }

    return Action::None;
}

void PauseMenu::draw(RenderWindow& window) const {
    window.draw(titleText);
    window.draw(resumeText);
    window.draw(optionsText);
    window.draw(restartText);
    window.draw(backText);
}

void PauseMenu::setFullscreen(bool enabled) {
    unsigned int titleSize = enabled ? 120 : 80;
    unsigned int itemSize = enabled ? 70 : 45;

    styleText(titleText, titleSize);
    styleText(resumeText, itemSize);
    styleText(optionsText, itemSize);
    styleText(restartText, itemSize);
    styleText(backText, itemSize);

    positionText(titleText, window, 0.20f);
    positionText(resumeText, window, 0.40f);
    positionText(optionsText, window, 0.50f);
    positionText(restartText, window, 0.60f);
    positionText(backText, window, 0.70f);
}

void PauseMenu::activate() {
    activationClock.restart();
    firstActivation = true;
}