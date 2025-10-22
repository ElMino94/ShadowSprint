#include "../../include/ui/OptionMenu.h"
#include "../../include/ui/UIUtils.h"

using namespace sf;
using namespace UIUtils;

OptionMenu::OptionMenu(RenderWindow& windowRef) : window(windowRef), font("../assets/fonts/samurai-blast.ttf"),
titleText(font), backText(font), musicVolumeText(font), sfxVolumeText(font),
musicLeftArrow(font), sfxLeftArrow(font), musicRightArrow(font), sfxRightArrow(font),
fullscreenText(font), vsyncText(font)
{
    titleText.setString("Options");
    musicVolumeText.setString("Musique : < 100% >");
    sfxVolumeText.setString("Effets : < 100% >");
    fullscreenText.setString("Plein écran : Oui");
    vsyncText.setString("VSync : Oui");
    backText.setString("Retour");

    musicLeftArrow.setString("<");
    musicRightArrow.setString(">");
    sfxLeftArrow.setString("<");
    sfxRightArrow.setString(">");

    styleText(titleText, 200);
    styleText(musicVolumeText, 80);
    styleText(sfxVolumeText, 80);
    styleText(fullscreenText, 80);
    styleText(vsyncText, 80);
    styleText(backText, 100);

    styleText(musicLeftArrow, 80);
    styleText(musicRightArrow, 80);
    styleText(sfxLeftArrow, 80);
    styleText(sfxRightArrow, 80);

    positionText(titleText, window, 0.10f);
    positionText(musicVolumeText, window, 0.30f);
    positionText(sfxVolumeText, window, 0.40f);
    positionText(fullscreenText, window, 0.55f);
    positionText(vsyncText, window, 0.65f);
    positionText(backText, window, 0.80f);

    updateVolumeText(true);
}

OptionMenu::OptionAction OptionMenu::handleEvent(const Event& event) {
    if (firstActivation && activationClock.getElapsedTime() < activationDelay) {
        return OptionAction::None;
    }
    firstActivation = false;

    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

            if (backText.getGlobalBounds().contains(mousePos)) return OptionAction::Back;

            if (fullscreenText.getGlobalBounds().contains(mousePos)) {
                fullscreen = !fullscreen;
                fullscreenText.setString("Plein écran : " + std::string(fullscreen ? "Oui" : "Non"));
                centerText(fullscreenText);
            }

            if (vsyncText.getGlobalBounds().contains(mousePos)) {
                vsync = !vsync;
                vsyncText.setString("VSync : " + std::string(vsync ? "Oui" : "Non"));
                centerText(vsyncText);
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

    styleText(titleText, titleSize);
    styleText(musicVolumeText, itemSize);
    styleText(sfxVolumeText, itemSize);
    styleText(fullscreenText, itemSize);
    styleText(vsyncText, itemSize);
    styleText(backText, backSize);

    styleText(musicLeftArrow, itemSize);
    styleText(musicRightArrow, itemSize);
    styleText(sfxLeftArrow, itemSize);
    styleText(sfxRightArrow, itemSize);

    positionText(titleText, window, 0.10f);
    positionText(musicVolumeText, window, 0.30f);
    positionText(sfxVolumeText, window, 0.40f);
    positionText(fullscreenText, window, 0.55f);
    positionText(vsyncText, window, 0.65f);
    positionText(backText, window, 0.80f);

    updateVolumeText(enabled);
}

void OptionMenu::updateVolumeText(bool fullscreen) {
    Vector2u windowSize = window.getSize();
    float musicY = windowSize.y * 0.30f;
    float sfxY = windowSize.y * 0.40f;

    std::ostringstream musicStream;
    musicStream << std::setw(3) << std::setfill(' ') << static_cast<int>(musicVolume);
    std::string musicFormatted = musicStream.str();

    musicVolumeText.setString("Musique :   " + musicFormatted + "%  ");
    centerText(musicVolumeText);
    musicVolumeText.setPosition({ windowSize.x / 2.f, musicY });

    float musicLeftOffset = fullscreen ? 80.f : 50.f;
    float musicRightOffset = fullscreen ? 350.f : 200.f;

    centerText(musicLeftArrow);
    musicLeftArrow.setPosition({ musicVolumeText.getPosition().x + musicLeftOffset, musicY });

    centerText(musicRightArrow);
    musicRightArrow.setPosition({ musicVolumeText.getPosition().x + musicRightOffset, musicY });

    std::ostringstream sfxStream;
    sfxStream << std::setw(3) << std::setfill(' ') << static_cast<int>(sfxVolume);
    std::string sfxFormatted = sfxStream.str();

    sfxVolumeText.setString("Effets :   " + sfxFormatted + "%  ");
    centerText(sfxVolumeText);
    sfxVolumeText.setPosition({ windowSize.x / 2.f, sfxY });

    float sfxLeftOffset = fullscreen ? 80.f : 50.f;
    float sfxRightOffset = fullscreen ? 350.f : 200.f;

    centerText(sfxLeftArrow);
    sfxLeftArrow.setPosition({ sfxVolumeText.getPosition().x + sfxLeftOffset, sfxY });

    centerText(sfxRightArrow);
    sfxRightArrow.setPosition({ sfxVolumeText.getPosition().x + sfxRightOffset, sfxY });
}