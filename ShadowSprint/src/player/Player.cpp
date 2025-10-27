#include "../../include/player/Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace sf;

Player::Player(float scale) : onGround(true), canDoubleJump(false), blocking(false),
    gravity(2500.f), jumpForce(1000.f), velocityY(0.f),
    currentState(State::Idle),
    playerScale(scale),
    idleTexture(), runTexture(), jumpTexture(), fallTexture(), blockTexture()
{
    if (!idleTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Idle.png"))
        std::cerr << "Erreur chargement Idle.png\n";
    if (!runTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Run.png"))
        std::cerr << "Erreur chargement Run.png\n";
    if (!jumpTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Jump.png"))
        std::cerr << "Erreur chargement Jump.png\n";
    if (!fallTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Fall.png"))
        std::cerr << "Erreur chargement Fall.png\n";
    if (!blockTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Block.png"))
        std::cerr << "Erreur chargement Block.png\n";

    idleAnim = Animation(idleTexture, 8, 0.15f);
    runAnim = Animation(runTexture, 8, 0.08f);
    jumpAnim = Animation(jumpTexture, 2, 0.10f);
    fallAnim = Animation(fallTexture, 2, 0.10f);
    blockAnim = Animation(blockTexture, 6, 0.12f, false);

    sprite = std::make_unique<sf::Sprite>(idleTexture);

    Vector2u texSize = idleTexture.getSize();
    sprite->setOrigin(Vector2f(
        static_cast<float>(texSize.x) / 8.f / 2.f,
        static_cast<float>(texSize.y)
    ));

    sprite->setScale(Vector2f(playerScale, playerScale));
    sprite->setPosition(Vector2f(300.f, 900.f));
}

void Player::handleInput()
{
    blocking = false;

    if (Keyboard::isKeyPressed(Keyboard::Scancode::Space)) {
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

    if (Keyboard::isKeyPressed(Keyboard::Scancode::LShift)) {
        blocking = true;
        setState(State::Blocking);
    }
}

void Player::jump()
{
    velocityY = -jumpForce;
}

void Player::update(float dt) {
    velocityY += gravity * dt;

    if (velocityY > 0.f && !onGround && currentState == State::Jumping) {
        setState(State::Falling);
    }

    if (sprite->getPosition().y >= 900.f) {
        sprite->setPosition(Vector2f(sprite->getPosition().x, 900.f));
        velocityY = 0.f;
        onGround = true;

        if (currentState == State::Jumping || currentState == State::Falling)
            setState(State::Running);
    }

    if (currentState == State::Blocking && blockAnim.isFinished()) {
        setState(State::Running);
    }

    switch (currentState) {
    case State::Idle:     idleAnim.update(*sprite, dt); break;
    case State::Running:  runAnim.update(*sprite, dt);  break;
    case State::Jumping:  jumpAnim.update(*sprite, dt); break;
    case State::Falling:  fallAnim.update(*sprite, dt); break;
    case State::Blocking: blockAnim.update(*sprite, dt); break;
    }
}

void Player::draw(RenderWindow& window) {
    window.draw(*sprite);
}

int Player::getFrameCountForState(State state) const {
    switch (state) {
    case State::Idle: return 8;
    case State::Running: return 8;
    case State::Jumping: return 2;
    case State::Falling: return 2;
    case State::Blocking: return 6;
    default: return 1;
    }
}

void Player::setState(State newState) {
    if (currentState != newState) {
        Vector2f oldPos = sprite->getPosition();
        float oldBottom = oldPos.y + sprite->getGlobalBounds().size.y;

        currentState = newState;

        Texture* newTexture = nullptr;

        switch (currentState) {
        case State::Idle:     idleAnim.reset();  newTexture = &idleTexture; break;
        case State::Running:  runAnim.reset();   newTexture = &runTexture;  break;
        case State::Jumping:  jumpAnim.reset();  newTexture = &jumpTexture; break;
        case State::Falling:  fallAnim.reset();  newTexture = &fallTexture; break;
        case State::Blocking: blockAnim.reset(); newTexture = &blockTexture; break;
        }

        if (newTexture) {
            int frameCount = getFrameCountForState(currentState);
            sprite->setTexture(*newTexture);

            Vector2u texSize = newTexture->getSize();
            float frameWidth = static_cast<float>(texSize.x) / frameCount;
            float frameHeight = static_cast<float>(texSize.y);

            sprite->setOrigin(Vector2f(frameWidth / 2.f, frameHeight));
            float newHeight = sprite->getGlobalBounds().size.y;
            sprite->setPosition(Vector2f(oldPos.x, oldBottom - newHeight));
        }
    }
}

void Player::reset() {
    setState(State::Idle);
    sprite->setPosition(Vector2f(300.f, 900.f));
    velocityY = 0.f;
    onGround = false;
    canDoubleJump = false;
    blocking = false;
}

void Player::activateInvincibility(float duration) {
    invincibilityEffect = { true, duration, 0.f };
    invincible = true;
}

void Player::activateSlowMode(float duration) {
    slowModeEffect = { true, duration, 0.f };
    slowMode = true;
}

void Player::activateScoreMultiplier(float multiplier, float duration) {
    activeMultipliers.push_back({ duration, 0.f });
}

float Player::getScoreMultiplier() const { return std::pow(2.f, static_cast<float>(activeMultipliers.size())); }
bool Player::isInvincible() const { return invincible; }
bool Player::isSlowMode() const { return slowMode; }

void Player::updateBonusTimer(float dt) {
    if (invincibilityEffect.active) {
        invincibilityEffect.timer += dt;
        if (invincibilityEffect.timer >= invincibilityEffect.duration) {
            invincibilityEffect.active = false;
            invincible = false;
        }
    }

    if (slowModeEffect.active) {
        slowModeEffect.timer += dt;
        if (slowModeEffect.timer >= slowModeEffect.duration) {
            slowModeEffect.active = false;
            slowMode = false;
        }
    }

    for (auto& m : activeMultipliers)
        m.timer += dt;

    activeMultipliers.erase(
        std::remove_if(activeMultipliers.begin(), activeMultipliers.end(),
            [](const MultiplierInstance& m) { return m.timer >= m.duration; }),
        activeMultipliers.end()
    );
}

FloatRect Player::getBounds() const { return sprite->getGlobalBounds(); }
void Player::move(const Vector2f& offset) { sprite->move(offset); }
void Player::setPosition(const Vector2f& pos) { sprite->setPosition(pos); }
Vector2f Player::getPosition() const { return sprite->getPosition(); }
bool Player::isBlocking() const { return blocking; }
bool Player::isOnGround() const { return onGround; }

void Player::setOnGround(bool v) { onGround = v; }
float Player::getVelocityY() const { return velocityY; }
void Player::setVelocityY(float v) { velocityY = v; }