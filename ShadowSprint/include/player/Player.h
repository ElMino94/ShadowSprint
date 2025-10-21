#pragma once
#include "../core/Entity.h"
#include "../core/Animation.h"

class Player : public Entity{
public:
	enum class State { Idle, Running, Jumping, Blocking };

	Player();

	void handleInput();
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;

	void setState(State newState);
	void reset();

	bool isBlocking() const;
	bool isOnGround() const;

private:

	Animation idleAnim;
	Animation runAnim;
	sf::Texture idleTexture;
	sf::Texture runTexture;

	State currentState;

	bool onGround;
	bool canDoubleJump;
	bool blocking;

	float gravity;
	float jumpForce;
	float velocityY;

	void jump();

};

