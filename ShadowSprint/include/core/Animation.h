#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

class Animation {
public:
    Animation() = default;

    Animation(Texture& texture, int frameCount, float frameTime);

    void update(Sprite& sprite, float deltaTime);
    void reset();

private:
    Texture* texture = nullptr;
    Vector2i frameSize = {0, 0};
    int frameCount = 0;
    float frameTime = 0.f;

    int currentFrame = 0;
    float timer = 0.f;
};
