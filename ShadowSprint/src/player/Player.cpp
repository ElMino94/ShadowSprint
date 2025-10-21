#include "../../include/player/Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Player::Player()
    : onGround(true),
    canDoubleJump(false),
    blocking(false),
    gravity(2500.f),
    jumpForce(1000.f),
    velocityY(0.f),
    currentState(State::Idle)
{
    if (!idleTexture.loadFromFile("../assets/Idle.png"))
        std::cerr << "Erreur chargement Idle.png\n";
    if (!runTexture.loadFromFile("../assets/Run.png"))
        std::cerr << "Erreur chargement Run.png\n";

    idleAnim = Animation(idleTexture, 8, 0.12f, { 512, 512 });
    runAnim = Animation(runTexture, 8, 0.08f, { 512, 512 });

    sprite.setTexture(idleTexture);
    sprite.setScale({ 0.25f, 0.25f });
    sprite.setPosition({ 300.f, 900.f });
}
void Player::handleInput() {
    blocking = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
        if (onGround) {
            jump();
            onGround = false;
            canDoubleJump = true;
            setState(State::Jumping);
        }
        else if (canDoubleJump) {
            jump();
            canDoubleJump = false;
            setState(State::Jumping);
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

        if (currentState == State::Jumping)
            setState(State::Running);
    }

    switch (currentState) {
    case State::Idle:
        idleAnim.update(dt);
        idleAnim.applyToSprite(sprite);
        break;
    case State::Running:
        runAnim.update(dt);
        runAnim.applyToSprite(sprite);
        break;
    case State::Blocking:
        idleAnim.applyToSprite(sprite);       // il ne faut pas laisser le idleanim
        break;
    case State::Jumping:
        runAnim.applyToSprite(sprite);       
        break;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::setState(State newState) {
    if (currentState != newState) {
        currentState = newState;

        switch (currentState) {
        case State::Idle: idleAnim.reset(); break;
        case State::Running: runAnim.reset(); break;
        default: break;
        }
    }
}

void Player::reset() {
    setState(State::Idle);
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