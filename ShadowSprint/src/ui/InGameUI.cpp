#include "../../include/ui/InGameUI.h"
#include "../../include/ui/Utils.h"

using namespace sf;
using namespace Utils;

InGameUI::InGameUI(sf::RenderWindow& windowRef) : window(windowRef), font("../assets/fonts/samurai-blast.ttf"),
distanceText(font), scoreBackgroundTexture("../assets/textures/roundedRectangle.png"), scoreBackgroundSprite(scoreBackgroundTexture)
{
    distanceText.setString("00000");

    styleText(distanceText, baseSize);
    positionTextTopRight(distanceText, window, 60.f, 150.f);

    scoreBackgroundSprite.setScale({ 0.5f, 0.5f });
    scoreBackgroundSprite.setOrigin(scoreBackgroundSprite.getLocalBounds().getCenter());
    scoreBackgroundSprite.setPosition(distanceText.getPosition());
}

void InGameUI::update(float dt, float distanceMeters) {
    int roundedDistance = static_cast<int>(distanceMeters);
    std::ostringstream oss;
    oss << std::setw(5) << std::setfill('0') << roundedDistance;
    distanceText.setString(oss.str());
    positionTextTopRight(distanceText, window, 60.f, 150.f);
    scoreBackgroundSprite.setPosition(distanceText.getPosition());
}

void InGameUI::draw(sf::RenderWindow& window) const {
    window.draw(scoreBackgroundSprite);
    window.draw(distanceText);
}

void InGameUI::setFullscreen(bool enabled) {
    float scale = enabled ? 0.5f : 0.3f;
    scoreBackgroundSprite.setScale({ scale, scale });
    scoreBackgroundSprite.setPosition(distanceText.getPosition());

    baseSize = enabled ? 40 : 24;

    styleText(distanceText, baseSize);
    positionTextTopRight(distanceText, window, 60.f, 150.f);
}