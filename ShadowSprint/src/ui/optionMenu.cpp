#include "../../include/ui/optionMenu.h"

OptionMenu::OptionMenu(RenderWindow& windowRef) : window(windowRef), font("../assets/fonts/samurai-blast.ttf"),
titleText(font), backText(font),
musicVolumeText(font), sfxVolumeText(font),
musicLeftArrow(font), sfxLeftArrow(font), musicRightArrow(font), sfxRightArrow(font),
fullscreenText(font), vsyncText(font)
{
    titleText.setString("Options");
    titleText.setCharacterSize(200);
    titleText.setFillColor(Color::White);
    titleText.setOrigin(titleText.getLocalBounds().getCenter());
    titleText.setPosition({ window.getSize().x / 2.f, 100.f });

    musicVolumeText.setString("Musique : < 100% >");
    musicVolumeText.setCharacterSize(80);
    musicVolumeText.setFillColor(Color::White);
    musicVolumeText.setOrigin(musicVolumeText.getLocalBounds().getCenter());
    musicVolumeText.setPosition({ window.getSize().x / 2.f, 300.f });

    sfxVolumeText.setString("Effets : < 100% >");
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

    musicLeftArrow.setFont(font);
    musicLeftArrow.setString("<");
    musicLeftArrow.setCharacterSize(80);
    musicLeftArrow.setFillColor(sf::Color::White);

    musicRightArrow.setFont(font);
    musicRightArrow.setString(">");
    musicRightArrow.setCharacterSize(80);
    musicRightArrow.setFillColor(sf::Color::White);

    sfxLeftArrow.setFont(font);
    sfxLeftArrow.setString("<");
    sfxLeftArrow.setCharacterSize(80);
    sfxLeftArrow.setFillColor(sf::Color::White);

    sfxRightArrow.setFont(font);
    sfxRightArrow.setString(">");
    sfxRightArrow.setCharacterSize(80);
    sfxRightArrow.setFillColor(sf::Color::White);

    updateVolumeText(true);
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

            if (musicLeftArrow.getGlobalBounds().contains(mousePos)) {
                musicVolume = std::max(0.f, musicVolume - 5.f);
                updateVolumeText(fullscreen);
            }
            if (musicRightArrow.getGlobalBounds().contains(mousePos)) {
                musicVolume = std::min(100.f, musicVolume + 5.f);
                updateVolumeText(fullscreen);
            }
            if (sfxLeftArrow.getGlobalBounds().contains(mousePos)) {
                sfxVolume = std::max(0.f, sfxVolume - 5.f);
                updateVolumeText(fullscreen);
            }
            if (sfxRightArrow.getGlobalBounds().contains(mousePos)) {
                sfxVolume = std::min(100.f, sfxVolume + 5.f);
                updateVolumeText(fullscreen);
            }
        }
    }

    return OptionAction::None;
}

void OptionMenu::update(float dt) {}

void OptionMenu::draw(RenderWindow& window) const {
    window.draw(titleText);
    window.draw(fullscreenText);
    window.draw(vsyncText);
    window.draw(backText);

    window.draw(musicLeftArrow);
    window.draw(musicVolumeText);
    window.draw(musicRightArrow);

    window.draw(sfxLeftArrow);
    window.draw(sfxVolumeText);
    window.draw(sfxRightArrow);

}

void OptionMenu::activate() {
    activationClock.restart();
    firstActivation = true;
    updateVolumeText(fullscreen);
}

bool OptionMenu::isFullscreenEnabled() const {
    return fullscreen;
}

bool OptionMenu::isVsyncEnabled() const {
    return vsync;
}

void OptionMenu::setFullscreen(bool enabled) {
    unsigned int titleSize = enabled ? 200 : 120;
    unsigned int itemSize = enabled ? 80 : 50;
    unsigned int backSize = enabled ? 100 : 70;

    titleText.setCharacterSize(titleSize);
    musicVolumeText.setCharacterSize(itemSize);
    sfxVolumeText.setCharacterSize(itemSize);
    fullscreenText.setCharacterSize(itemSize);
    vsyncText.setCharacterSize(itemSize);
    backText.setCharacterSize(backSize);
    musicLeftArrow.setCharacterSize(itemSize);
    musicRightArrow.setCharacterSize(itemSize);
    sfxLeftArrow.setCharacterSize(itemSize);
    sfxRightArrow.setCharacterSize(itemSize);

    Vector2u windowSize = window.getSize();

    titleText.setOrigin(titleText.getLocalBounds().getCenter());
    titleText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.10f });

    musicVolumeText.setOrigin(musicVolumeText.getLocalBounds().getCenter());
    musicVolumeText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.30f });

    sfxVolumeText.setOrigin(sfxVolumeText.getLocalBounds().getCenter());
    sfxVolumeText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.40f });

    musicLeftArrow.setOrigin(musicLeftArrow.getLocalBounds().getCenter());
    musicLeftArrow.setPosition({ musicVolumeText.getPosition().x + 50.f, musicVolumeText.getPosition().y });

    musicRightArrow.setOrigin(musicRightArrow.getLocalBounds().getCenter());
    musicRightArrow.setPosition({ musicVolumeText.getPosition().x + 200.f, musicVolumeText.getPosition().y });

    sfxLeftArrow.setOrigin(sfxLeftArrow.getLocalBounds().getCenter());
    sfxLeftArrow.setPosition({ sfxVolumeText.getPosition().x + 50.f, sfxVolumeText.getPosition().y });

    sfxRightArrow.setOrigin(sfxRightArrow.getLocalBounds().getCenter());
    sfxRightArrow.setPosition({ sfxVolumeText.getPosition().x + 200.f, sfxVolumeText.getPosition().y });

    fullscreenText.setOrigin(fullscreenText.getLocalBounds().getCenter());
    fullscreenText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.55f });

    vsyncText.setOrigin(vsyncText.getLocalBounds().getCenter());
    vsyncText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.65f });

    backText.setOrigin(backText.getLocalBounds().getCenter());
    backText.setPosition({ windowSize.x / 2.f, windowSize.y * 0.80f });
}

void OptionMenu::updateVolumeText(bool fullscreen) {
    Vector2u windowSize = window.getSize();
    float musicY = windowSize.y * 0.30f;
    float sfxY = windowSize.y * 0.40f;

    std::ostringstream musicStream;
    musicStream << std::setw(3) << std::setfill(' ') << static_cast<int>(musicVolume);
    std::string musicFormatted = musicStream.str();

    musicVolumeText.setString("Musique :   " + musicFormatted + "%  ");
    musicVolumeText.setOrigin(musicVolumeText.getLocalBounds().getCenter());
    musicVolumeText.setPosition({ windowSize.x / 2.f, musicY });

    float musicLeftOffset = fullscreen ? 80.f : 50.f;
    float musicRightOffset = fullscreen ? 350.f : 200.f;

    musicLeftArrow.setOrigin(musicLeftArrow.getLocalBounds().getCenter());
    musicLeftArrow.setPosition({ musicVolumeText.getPosition().x + musicLeftOffset, musicY });

    musicRightArrow.setOrigin(musicRightArrow.getLocalBounds().getCenter());
    musicRightArrow.setPosition({ musicVolumeText.getPosition().x + musicRightOffset, musicY });

    std::ostringstream sfxStream;
    sfxStream << std::setw(3) << std::setfill(' ') << static_cast<int>(sfxVolume);
    std::string sfxFormatted = sfxStream.str();

    sfxVolumeText.setString("Effets :   " + sfxFormatted + "%  ");
    sfxVolumeText.setOrigin(sfxVolumeText.getLocalBounds().getCenter());
    sfxVolumeText.setPosition({ windowSize.x / 2.f, sfxY });

    float sfxLeftOffset = fullscreen ? 80.f : 50.f;
    float sfxRightOffset = fullscreen ? 350.f : 200.f;

    sfxLeftArrow.setOrigin(sfxLeftArrow.getLocalBounds().getCenter());
    sfxLeftArrow.setPosition({ sfxVolumeText.getPosition().x + sfxLeftOffset, sfxY });

    sfxRightArrow.setOrigin(sfxRightArrow.getLocalBounds().getCenter());
    sfxRightArrow.setPosition({ sfxVolumeText.getPosition().x + sfxRightOffset, sfxY });
}