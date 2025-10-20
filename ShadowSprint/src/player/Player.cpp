#include "../../include/player/Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Player::Player()
    : onGround(true),
    canDoubleJump(false),
    blocking(false),
    gravity(2500.f),
    jumpForce(1000.f),
    velocityY(0.f)
{
    if (!texture.loadFromFile("../assets/player.png")) {
        std::cerr << "Erreur : impossible de charger ../assets/player.png\n";
    }

    sprite.setTexture(texture);
    sprite.setScale({ 0.3f, 0.3f });
    sprite.setPosition({ 300.f, 900.f }); 
}

void Player::handleInput() {
    blocking = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
        if (onGround) {
            jump();
            onGround = false;
            canDoubleJump = true;
        }
        else if (canDoubleJump) {
            jump();
            canDoubleJump = false;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {
        blocking = true;
    }
}

void Player::jump() {
    velocityY = -jumpForce;
}

void Player::update(float dt) {
    velocityY += gravity * dt;
    move({ 0.f, velocityY * dt });

    if (getPosition().y >= 900.f) {
        setPosition({ getPosition().x, 900.f });
        velocityY = 0.f;
        onGround = true;
    }
}

void Player::reset() {
    setPosition({ 300.f, 900.f });
    velocityY = 0.f;
    onGround = true;
    canDoubleJump = false;
    blocking = false;
}

bool Player::isBlocking() const {
    return blocking;
}

bool Player::isOnGround() const {
    return onGround;
}
