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
    playerScale(scale),
    jumpForwardSpeed(350.f),
    idleTexture(), runTexture(), jumpTexture(), blockTexture(),
    sprite(idleTexture) 
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
    sprite.setOrigin(sf::Vector2f(
        static_cast<float>(texSize.x) / 8.f / 2.f,
        static_cast<float>(texSize.y)
    ));

    sprite.setScale(sf::Vector2f(playerScale, playerScale));
    sprite.setPosition(sf::Vector2f(300.f, 900.f));
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
        setState(State::Blocking);
    }
}

void Player::jump() {
    velocityY = -jumpForce;

}

void Player::update(float dt) {
    velocityY += gravity * dt;

    if (currentState == State::Jumping) {
        sprite.move(sf::Vector2f(jumpForwardSpeed * dt, velocityY * dt));
    }
    else {
        sprite.move(sf::Vector2f(0.f, velocityY * dt));
    }

    if (sprite.getPosition().y >= 900.f) {
        sprite.setPosition(sf::Vector2f(sprite.getPosition().x, 900.f));
        velocityY = 0.f;
        onGround = true;

        if (currentState == State::Jumping)
            setState(State::Running);
    }

    // Anim
    switch (currentState) {
    case State::Idle:     idleAnim.update(sprite, dt); break;
    case State::Running:  runAnim.update(sprite, dt);  break;
    case State::Jumping:  jumpAnim.update(sprite, dt); break;
    case State::Blocking: blockAnim.update(sprite, dt); break;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::setState(State newState) {
    if (currentState != newState) {
        sf::Vector2f oldPos = sprite.getPosition();
        float oldBottom = oldPos.y + sprite.getGlobalBounds().size.y;

        currentState = newState;

        switch (currentState) {
        case State::Idle:     idleAnim.reset();  sprite.setTexture(idleTexture);  break;
        case State::Running:  runAnim.reset();   sprite.setTexture(runTexture);   break;
        case State::Jumping:  jumpAnim.reset();  sprite.setTexture(jumpTexture);  break;
        case State::Blocking: blockAnim.reset(); sprite.setTexture(blockTexture); break;
        }

        float newHeight = sprite.getGlobalBounds().size.y;
        sprite.setPosition(sf::Vector2f(oldPos.x, oldBottom - newHeight));

        if (currentState == State::Jumping) {
            
        }
    }
}

void Player::move(const sf::Vector2f& offset) { sprite.move(offset); }
void Player::setPosition(const sf::Vector2f& pos) { sprite.setPosition(pos); }
sf::Vector2f Player::getPosition() const { return sprite.getPosition(); }

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::reset() {
    setState(State::Idle);
    sprite.setPosition(sf::Vector2f(300.f, 900.f));
    velocityY = 0.f;
    onGround = true;
    canDoubleJump = false;
    blocking = false;
}

bool Player::isBlocking() const { return blocking; }
bool Player::isOnGround() const { return onGround; }
