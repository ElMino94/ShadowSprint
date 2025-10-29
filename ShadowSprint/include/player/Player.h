#pragma once
#include "../core/Entity.h"
#include "../core/Animation.h"

class Player : public Entity {
public:
    enum class State { Idle, Running, Jumping, Falling, Blocking };

    Player(float scale = 3.f);

    void handleInput();
    void update(float dt) override;
    void updateSpeed(float dt, float score);
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
    void setOnGround(bool v);
    float getVelocityY() const;
    void setVelocityY(float v);

    //Bonus
    void updateBonusTimer(float dt);

    void activateInvincibility(float duration);
    void activateSlowMode(float duration);
    void activateScoreMultiplier(float multiplier, float duration);
    float getScoreMultiplier() const;
    bool isInvincible() const;
    bool isSlowMode() const;

    float getBlockCooldown() const;
    float getBlockCooldownDuration() const;

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
    std::unique_ptr<sf::Sprite> sprite;

    State  currentState;
    bool   onGround;
    bool   canDoubleJump;
    bool   blocking;
    float  gravity;
    float  jumpForce;
    float  velocityY;
    float  playerScale;
    float blockCooldown;
    const float blockCooldownDuration;

    void jump();

    struct BonusEffect {
        bool active = false;
        float duration = 0.f;
        float timer = 0.f;
    };

    BonusEffect invincibilityEffect;
    BonusEffect slowModeEffect;

    bool invincible = false;
    bool slowMode = false;

    struct MultiplierInstance {
        float duration;
        float timer;
    };

    std::vector<MultiplierInstance> activeMultipliers;
};