#pragma once
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>

class OptionMenu {
public:
    enum class OptionAction {
        None,
        Back
    };

    OptionMenu(sf::RenderWindow& window);
    OptionAction handleEvent(const sf::Event& event);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    void activate();
    void setFullscreen(bool enabled);

    bool isFullscreenEnabled() const;
    bool isVsyncEnabled() const;
    void updateVolumeText(bool fullscreen);

private:
    sf::RenderWindow& window;

    sf::Font font;
    sf::Text titleText;
    sf::Text backText;

    sf::Text musicVolumeText;
    sf::Text sfxVolumeText;
    sf::Text musicLeftArrow;
    sf::Text musicRightArrow;
    sf::Text sfxLeftArrow;
    sf::Text sfxRightArrow;

    sf::Text fullscreenText;
    sf::Text vsyncText;

    float musicVolume = 100.f;
    float sfxVolume = 100.f;

    bool fullscreen = true;
    bool vsync = true;

    sf::Clock activationClock;
    bool firstActivation = true;
    const sf::Time activationDelay = sf::milliseconds(200);
};