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
    idleTexture(), runTexture(), jumpTexture(), blockTexture(),
    sprite(idleTexture),
    idleTestTexture("../assets/textures/Martial Hero/Sprites/idle_test.png"),
    idleTestSprite(idleTestTexture)
{
    if (!idleTestTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/idle_test.png"))
        std::cerr << "Erreur chargement Idle Test.png\n";

    /*    if (!idleTexture.loadFromFile("../assets/textures/Martial Hero/Sprites/Idle.png"))
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
    ));*/

    idleTestSprite.setOrigin(idleTestSprite.getLocalBounds().getCenter());

    idleTestSprite.setScale(sf::Vector2f(playerScale, playerScale));
    idleTestSprite.setPosition(sf::Vector2f(300.f, 900.f));
}

void Player::handleInput()
{
    blocking = false;

    // Jump
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

    // Block
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {
        blocking = true;
        setState(State::Blocking);
    }
}

void Player::jump()
{
    velocityY = -jumpForce;
}

void Player::update(float dt)
{
    // Gravity
    velocityY += gravity * dt;
    idleTestSprite.move({ 0.f, velocityY * dt });
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(idleTestSprite);
}

void Player::setState(State newState)
{
    if (currentState != newState) {
        currentState = newState;
        switch (currentState) {
        case State::Idle:     std::cout << "Idle\n"; break;
        case State::Running:  std::cout << "Running\n"; break;
        case State::Jumping:  std::cout << "Jumping\n"; break;
        case State::Blocking: std::cout << "Blocking\n"; break;
        }
    }
}

void Player::move(const sf::Vector2f& offset) { 
    idleTestSprite.move(offset); 
}

void Player::setPosition(const sf::Vector2f& pos) { 
    idleTestSprite.setPosition(pos); 
}

sf::Vector2f Player::getPosition() const { 
    return idleTestSprite.getPosition(); 
}

sf::FloatRect Player::getBounds() const {
    sf::Vector2f pos = idleTestSprite.getPosition();
    const sf::Vector2f hitboxSize = { 60.f, 100.f };

    sf::Vector2f topLeft(pos.x - hitboxSize.x / 2.f, pos.y - hitboxSize.y / 2.f);
    return sf::FloatRect(topLeft, hitboxSize);
}

void Player::reset()
{
    setState(State::Idle);
    idleTestSprite.setPosition({ 300.f, 0.f });
    velocityY = 0.f;
    onGround = false;
    canDoubleJump = false;
    blocking = false;
}

bool Player::isBlocking() const { 
    return blocking;
}

bool Player::isOnGround() const { 
    return onGround; 
}

void Player::setOnGround(bool v) { 
    onGround = v; 
}

float Player::getVelocityY() const { 
    return velocityY;
}

void Player::setVelocityY(float v) { 
    velocityY = v;
}