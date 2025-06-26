#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Player;
class ItemData;
class ItemManager;

#include "AnimationManager.hpp"

class Enemy {
protected:
    sf::FloatRect hitbox;

    int           state;
    float         MOVE_SPEED; 
    sf::Vector2f  basePosition;
    sf::Vector2f  movingDirection;

    int                TEXT_SIZE;
    float              BACKGROUND_PADDING;
    sf::Text           label;
    sf::RectangleShape labelBackground;

    float              HEALTH_POINTS_BAR_WIDTH;
    float              HEALTH_POINTS_BAR_HEIGHT;
    float              maxHealthPoints;
    float              healthPoints;
    sf::RectangleShape healthPointsBar;
    sf::RectangleShape healthPointsBarBackground;

    float DYING_TIME;
    float dyingCooldownTimer;

    float RESPAWN_TIME;
    float respawnCooldownTimer;
    
    float INVINCIBLE_TIME;
    float invincibleCooldownTimer;

    float DETECION_RANGE;
    float ALERT_LIFETIME;
    float alertCooldownTimer;

    float RANDOM_TIME;
    float randomCooldownTimer;
    float stayingCooldownTimer;

    float ATTACK_COOLDOWN_TIME;
    float attackCooldownTimer;

    sf::CircleShape  detectionBox;
    AnimationManager animationManager;
    Animation        shadow;
    Animation        alert;

    float KNOCKBACK_STRENGTH;
    float KNOCKBACK_COOLDOWN;
    float knockbackCooldownTimer;

    std::vector<std::pair<float, std::shared_ptr<ItemData>>> inventory; // rate - item

    float damagePerAttack;
    float expAmount;

public:
    Enemy(const sf::Vector2f& position, 
          const sf::Vector2f& size, 
          const std::string&  nameAndLevel,
          const float&        hp, 
          const float&        damage,
          const float&        exp,
          const std::vector<std::pair<float, std::shared_ptr<ItemData>>>& _inventory);

    virtual ~Enemy() = default;

    bool isAlive() const;
    void attack(Player& player);
    void hurt(const float& damage);
    void knockback(const sf::Vector2f& playerPosition);
    void kill();
    virtual void respawn();
    bool isDespawn() const;

    float calculateDistance(const Player& player) const;

    virtual void updateTimer(const float &dt);
    virtual void followPlayer(const Player& player);
    void moveRandomly();
    void updateThinking(Player& player);
    void updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects);
    void updateHitbox();
    void dropItems(ItemManager& items);
    virtual void updateAnimation() = 0;
    virtual void update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects, ItemManager& items);

    virtual void draw(sf::RenderTarget& target);

    sf::FloatRect getHitbox() const;
};