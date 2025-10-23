#pragma once
#include "../core/Entity.h"
#include "../core/Animation.h"

class Player : public Entity {
public:
    enum class State { Idle, Running, Jumping, Falling, Blocking };

    Player(float scale = 3.f);

    void handleInput();
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void move(const sf::Vector2f& offset) override;
    void setPosition(const sf::Vector2f& pos) override;
    sf::Vector2f getPosition() const override;
    sf::FloatRect getBounds() const override;

    int getFrameCountForState(State state) const;
    void setState(State newState);
    void reset();

    bool isBlocking() const;
    bool isOnGround() const;

private:
    Animation idleAnim;
    Animation runAnim;
    Animation jumpAnim;
    Animation fallAnim;
    Animation blockAnim;

    sf::Texture idleTexture;
    sf::Texture runTexture;
    sf::Texture jumpTexture;
    sf::Texture fallTexture;
    sf::Texture blockTexture;
    sf::Sprite  sprite;

    State  currentState;
    bool   onGround;
    bool   canDoubleJump;
    bool   blocking;
    float  gravity;
    float  jumpForce;
    float  velocityY;
    float  playerScale;

    void jump();
};
