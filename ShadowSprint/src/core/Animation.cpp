#include "../../include/core/Animation.h"

Animation::Animation(Texture& tex,  int frameCount, float frameTime)
    : texture(&tex),
    frameCount(frameCount),
    frameTime(frameTime),
    currentFrame(0),
    timer(0.f)
{
    Vector2u texSize = texture->getSize();
    frameSize = Vector2i(texSize.x / frameCount, texSize.y);
}

void Animation::update(Sprite& sprite, float deltaTime) {
    timer += deltaTime;

    if (timer >= frameTime) {
        timer = 0.f;
        currentFrame++;

        if (currentFrame >= frameCount)
            currentFrame = 0; 
    }

    IntRect rect(
        Vector2i(currentFrame * frameSize.x, 0),
        frameSize
    );

    sprite.setTexture(*texture);
    sprite.setTextureRect(rect);
}

void Animation::reset() {
    currentFrame = 0;
    timer = 0.f;
}
