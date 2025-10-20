#include "../../include/ui/optionMenu.h"

OptionMenu::OptionMenu(RenderWindow& windowRef) : window(windowRef), font("../assets/fonts/samurai-blast.ttf"),
titleText(font), backText(font),
musicVolumeText(font), sfxVolumeText(font),
fullscreenText(font), vsyncText(font)
{
    titleText.setString("Options");
    titleText.setCharacterSize(200);
    titleText.setFillColor(Color::White);
    titleText.setOrigin(titleText.getLocalBounds().getCenter());
    titleText.setPosition({ window.getSize().x / 2.f, 100.f });

    musicVolumeText.setString("Musique : 100%");
    musicVolumeText.setCharacterSize(80);
    musicVolumeText.setFillColor(Color::White);
    musicVolumeText.setOrigin(musicVolumeText.getLocalBounds().getCenter());
    musicVolumeText.setPosition({ window.getSize().x / 2.f, 300.f });

    sfxVolumeText.setString("Effets : 100%");
    sfxVolumeText.setCharacterSize(80);
    sfxVolumeText.setFillColor(Color::White);
    sfxVolumeText.setOrigin(sfxVolumeText.getLocalBounds().getCenter());
    sfxVolumeText.setPosition({ window.getSize().x / 2.f, 400.f });

    fullscreenText.setString("Plein écran : Oui");
    fullscreenText.setCharacterSize(80);
    fullscreenText.setFillColor(Color::White);
    fullscreenText.setOrigin(fullscreenText.getLocalBounds().getCenter());
    fullscreenText.setPosition({ window.getSize().x / 2.f, 550.f });

    vsyncText.setString("VSync : Oui");
    vsyncText.setCharacterSize(80);
    vsyncText.setFillColor(Color::White);
    vsyncText.setOrigin(vsyncText.getLocalBounds().getCenter());
    vsyncText.setPosition({ window.getSize().x / 2.f, 650.f });

    backText.setString("Retour");
    backText.setCharacterSize(100);
    backText.setFillColor(Color::White);
    backText.setOrigin(backText.getLocalBounds().getCenter());
    backText.setPosition({ window.getSize().x / 2.f, 800.f });
}

OptionMenu::OptionAction OptionMenu::handleEvent(const Event& event, RenderWindow& window) {
    if (firstActivation && activationClock.getElapsedTime() < activationDelay) {
        return OptionAction::None;
    }
    firstActivation = false;

    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

            if (backText.getGlobalBounds().contains(mousePos)) {
                return OptionAction::Back;
            }

            if (fullscreenText.getGlobalBounds().contains(mousePos)) {
                fullscreen = !fullscreen;
                fullscreenText.setString("Plein écran : " + std::string(fullscreen ? "Oui" : "Non"));
                fullscreenText.setOrigin(fullscreenText.getLocalBounds().getCenter());
            }

            if (vsyncText.getGlobalBounds().contains(mousePos)) {
                vsync = !vsync;
                vsyncText.setString("VSync : " + std::string(vsync ? "Oui" : "Non"));
                vsyncText.setOrigin(vsyncText.getLocalBounds().getCenter());
            }
        }
    }

    return OptionAction::None;
}

void OptionMenu::update(float dt) {}

void OptionMenu::draw(RenderWindow& window) const {
    window.draw(titleText);
    window.draw(musicVolumeText);
    window.draw(sfxVolumeText);
    window.draw(fullscreenText);
    window.draw(vsyncText);
    window.draw(backText);
}

void OptionMenu::activate() {
    activationClock.restart();
    firstActivation = true;
}

bool OptionMenu::isFullscreenEnabled() const {
    return fullscreen;
}

bool OptionMenu::isVsyncEnabled() const {
    return vsync;
}