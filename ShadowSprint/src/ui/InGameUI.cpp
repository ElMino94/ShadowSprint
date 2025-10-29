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
    updateBonusIcons(dt);

    float ratio = std::clamp(1.f - (blockCooldown / blockCooldownMax), 0.f, 1.f);
    blockCooldownBar.setSize(sf::Vector2f(200.f * ratio, 20.f));
    blockCooldownBar.setFillColor(sf::Color::Blue);
    blockCooldownBar.setPosition({ 50.f, 50.f });

    background.setSize(sf::Vector2f(200.f, 20.f));
    background.setFillColor(sf::Color(50, 50, 50, 150));
    background.setPosition({ 50.f, 50.f });
}

void InGameUI::draw(sf::RenderWindow& window) const {
    window.draw(scoreBackgroundSprite);
    window.draw(distanceText);
    for (const auto& icon : activeBonusIcons)
        window.draw(icon.sprite);
    window.draw(background);
    window.draw(blockCooldownBar);
}

void InGameUI::addBonusIcon(const sf::Texture& texture, float duration) {
    activeBonusIcons.emplace_back(texture, duration);
}

void InGameUI::updateBonusIcons(float dt) {
    for (auto& icon : activeBonusIcons)
        icon.remainingTime -= dt;

    activeBonusIcons.erase(
        std::remove_if(activeBonusIcons.begin(), activeBonusIcons.end(),
            [](const ActiveBonusIcon& icon) { return icon.remainingTime <= 0.f; }),
        activeBonusIcons.end()
    );

    sf::Vector2u size = window.getSize();
    float startX = size.x - 150.f;
    float startY = 120.f;

    for (size_t i = 0; i < activeBonusIcons.size(); ++i) {
        activeBonusIcons[i].sprite.setPosition({ startX, startY + i *100.f });
    }
}

void InGameUI::setBlockCooldown(float current, float max) {
    blockCooldown = current;
    blockCooldownMax = max;
}