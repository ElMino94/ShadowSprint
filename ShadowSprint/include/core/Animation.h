#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Animation {
public:
    Animation();
    Animation(Texture& tex, int frameCount, float frameTime, bool loop = true);

    void update(Sprite& sprite, float deltaTime);
    void reset();
    bool isFinished() const;
    void setFrameTime(float newTime) { frameTime = newTime; }

private:
    Texture* texture;
    int frameCount;
    float frameTime;
    float timer;
    int currentFrame;
    Vector2i frameSize;
    bool loop;
};