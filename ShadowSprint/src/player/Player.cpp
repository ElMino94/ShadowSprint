#include "../../include/player/Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Player::Player(float scale)
    : onGround(true),
    canDoubleJump(false),
    blocking(false),
    gravity(2500.f),
    jumpForce(1000.f),
    velocityY(0.f),
    currentState(State::Idle),
    playerScale(scale)
{
    if (!idleTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Idle.png"))
        std::cerr << "Erreur chargement Idle.png\n";
    if (!runTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Run.png"))
        std::cerr << "Erreur chargement Run.png\n";
    if (!jumpTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Jump.png"))
        std::cerr << "Erreur chargement Jump.png\n";
    if (!blockTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Block.png"))
        std::cerr << "Erreur chargement Block.png\n";

    idleAnim = Animation(idleTexture, 8, 0.15f);
    runAnim = Animation(runTexture, 8, 0.08f);
    jumpAnim = Animation(jumpTexture, 6, 0.10f);
    blockAnim = Animation(blockTexture, 4, 0.12f);

    sprite.setTexture(idleTexture);
    sf::Vector2u texSize = idleTexture.getSize();
    sprite.setOrigin(
        sf::Vector2f((texSize.x / 8.f) / 2.f, texSize.y)
    );

    sprite.setScale({ playerScale, playerScale });
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
        idleAnim.update(sprite, dt);
        break;
    case State::Running:
        runAnim.update(sprite, dt);
        break;
    case State::Jumping:
        jumpAnim.update(sprite, dt);
        break;
    case State::Blocking:
        blockAnim.update(sprite, dt);
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