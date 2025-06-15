#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.hpp"
#include "Projectile.hpp"

enum class EyeState {
    DYING = -2,
    DEAD  = -1,
    IDLE_LEFT,
    IDLE_RIGHT
};

class Eye : public Enemy {
private:
    Projectile projectile;
    float      SHOOT_COOLDOWN;
    float      PROJECTILE_SPEED;
    float      PROJECTILE_LIFETIME;
    float      shootCooldownTimer;

public:
    Eye(const float& x, const float& y);

    void respawn() override;
    void kill() override;

    void updateTimer(const float &dt) override;
    void followPlayer(const Player& player) override;
    void updateAnimation() override;
    void updateProjectiles(const float& dt, Player& player);
    void update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects) override;

    void draw(sf::RenderTarget& target) override;

};