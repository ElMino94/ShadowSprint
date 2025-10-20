#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class OptionMenu {
public:
    enum class OptionAction {
        None,
        Back
    };

    OptionMenu(RenderWindow& window);
    OptionAction handleEvent(const Event& event, RenderWindow& window);
    void update(float dt);
    void draw(RenderWindow& window) const;

    void activate();

    bool isFullscreenEnabled() const;
    bool isVsyncEnabled() const;

private:
    RenderWindow& window;

    Font font;
    Text titleText;
    Text backText;

    Text musicVolumeText;
    Text sfxVolumeText;

    Text fullscreenText;
    Text vsyncText;

    float musicVolume = 100.f;
    float sfxVolume = 100.f;
    bool fullscreen = true;
    bool vsync = true;

    Clock activationClock;
    bool firstActivation = true;
    const Time activationDelay = milliseconds(200);
};