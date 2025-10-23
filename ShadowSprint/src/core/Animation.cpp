#include "../../include/core/Animation.h"

Animation::Animation(Texture& tex, int frameCount, float frameTime, bool loop)
    : texture(&tex),
    frameCount(frameCount),
    frameTime(frameTime),
    timer(0.f),
    currentFrame(0),
    loop(loop)
{
    Vector2u texSize = texture->getSize();
    frameSize = Vector2i(texSize.x / frameCount, texSize.y);
}

void Animation::update(Sprite& sprite, float deltaTime) {
    timer += deltaTime;

    if (timer >= frameTime) {
        timer = 0.f;

        if (loop) {
            currentFrame = (currentFrame + 1) % frameCount;
        }
        else if (currentFrame < frameCount - 1) {
            currentFrame++;
        }
    }

    IntRect rect(Vector2i(currentFrame * frameSize.x, 0), frameSize);
    sprite.setTexture(*texture);
    sprite.setTextureRect(rect);
}

void Animation::reset() {
    timer = 0.f;
    currentFrame = 0;
}

bool Animation::isFinished() const {
    return !loop && currentFrame >= frameCount - 1;
}