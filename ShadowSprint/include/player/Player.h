#pragma once
#include "../core/Entity.h"
#include "../core/Animation.h"

class Player : public Entity{

private:

	Animation idleAnim;
	Animation runAnim;
	sf::Texture idleTexture;
	sf::Texture runTexture;

	bool onGround;
	bool canDoubleJump;
	bool blocking;

	float gravity;
	float jumpForce;
	float velocityY;

	void jump();

public:

	Player();

	void handleInput();
	void update(float dt) override;
	void reset();

	bool isBlocking() const;
	bool isOnGround() const;

};

