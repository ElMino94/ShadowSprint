#include "../core/Animation.h"

Animation::Animation()
	: texture(nullptr), currentFrame(0), frameTime(0.1f), elapsed(0.f){ }

Animation::Animation(sf::Texture& tex, int frameCount, float frameTime, sf::Vector2i frameSize)
	: texture(&tex), frameTime(frameTime), currentFrame(0), elapsed(0.f) {
	for (int i = 0; i < frameCount; i++) {
		frames.emplace_back(i * frameSize.x, 0, frameSize.x, frameSize.y);
	}
}

void Animation::update(float dt) {
	if (!texture || frames.empty())return;

	elapsed += dt;
	if (elapsed >= frameTime) {
		elapsed = 0.f;
		currentFrame = (currentFrame + 1) % frames.size();
	}
}

void Animation::applyToSprite(sf::Sprite& sprite) {
	if (texture && !frames.empty()) {
		sprite.setTexture(*texture);
		sprite.setTextureRect(frames[currentFrame]);
	}
}

void Animation::reset() {
	currentFrame = 0;
	elapsed = 0.f;
}
