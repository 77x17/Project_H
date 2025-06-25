#include "Eye.hpp"

#include "Player.hpp"
#include "ItemData.hpp"
#include "ItemManager.hpp"

#include "Constants.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"
#include "Normalize.hpp"

Eye::Eye(const sf::Vector2f& position, 
         const std::string&  nameAndLevel,
         const float&        hp, 
         const std::vector<std::pair<float, std::shared_ptr<ItemData>>>& _inventory) 
: Enemy(position, sf::Vector2f(TILE_SIZE, TILE_SIZE), hp, nameAndLevel, _inventory) {
    // --- [Begin] - Configuration ---
    MOVE_SPEED      = 80.0f; 
    
    DETECION_RANGE  = 250.0f;

    damagePerAttack = 2.5f;
    expAmount       = 2.5f;
    // --- [End] ---

    // --- [Begin] - Animation ---
    detectionBox.setRadius(DETECION_RANGE);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    animationManager.addAnimation(static_cast<int>(EyeState::IDLE_LEFT) , TextureManager::get("eyeSprite"), 20, 20, 2, 0, 0.5f, true );
    animationManager.addAnimation(static_cast<int>(EyeState::IDLE_RIGHT), TextureManager::get("eyeSprite"), 20, 20, 2, 0, 0.5f, false);
    animationManager.addAnimation(static_cast<int>(EyeState::DYING)     , TextureManager::get("eyeDead")  , 20, 20, 1, 0, 0.5f, false);

    shadow = Animation(TextureManager::get("eyeShadow"), 12,  5, 1, 0, 0.f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.f, false);
    // --- [End] ---

    // --- [Begin] - Projectile ---
    SHOOT_COOLDOWN      = 2.0f;
    PROJECTILE_SPEED    = MOVE_SPEED * 1.5;
    PROJECTILE_LIFETIME = 1.0f;
    shootCooldownTimer  = 0.0f;
    // --- [End] ---
}

// void Eye::respawn() {
//     if (state == static_cast<int>(EyeState::DEAD) && respawnCooldownTimer <= 0) {
//         Enemy::respawn();
//     }
// }

void Eye::updateTimer(const float &dt) {
    Enemy::updateTimer(dt);

    if (shootCooldownTimer > 0) {
        shootCooldownTimer -= dt;
    }
}

void Eye::followPlayer(const Player& player) {
    sf::Vector2f normalizeDirection = Normalize::normalize(player.getPosition() - hitbox.getPosition());

    if (shootCooldownTimer <= 0) {
        projectile = std::make_unique<Projectile>(
            TextureManager::get("fireball"),
            hitbox.getPosition() + hitbox.getSize() / 2.f,
            normalizeDirection,
            PROJECTILE_SPEED,
            PROJECTILE_LIFETIME
        );

        SoundManager::playSound("fireball");

        shootCooldownTimer = SHOOT_COOLDOWN;
    }
    else if (shootCooldownTimer < 0.2f) {
        alertCooldownTimer = ALERT_LIFETIME;
    }   

    // movingDirection = normalizeDirection;

    Enemy::moveRandomly();
}

void Eye::updateAnimation() {
    if (!isAlive() || knockbackCooldownTimer > 0) {
        // nothing
    }
    else if (movingDirection.x < 0) {
        state = static_cast<int>(EyeState::IDLE_LEFT);
    }
    else if (movingDirection.x > 0) {
        state = static_cast<int>(EyeState::IDLE_RIGHT);
    }

    animationManager.setState(state, true);
    animationManager.setPosition(hitbox.getPosition() - sf::Vector2f(4, 4));
    animationManager.update();

    alert .setPosition(hitbox.getPosition() + sf::Vector2f(8, -15));
    shadow.setPosition(hitbox.getPosition() + sf::Vector2f(4, hitbox.getSize().y - 8));
}

void Eye::updateProjectiles(const float& dt, Player& player) {
    if (!projectile) {
        return;
    }

    projectile->update(dt);

    if (projectile->isAlive()) {
        if (projectile->isCollision(player.getHitbox())) {
            player.hurt(damagePerAttack);
            player.knockback(projectile->getPosition());

            projectile.reset();
        }
    }
    else {
        projectile.reset();
    }
}   

void Eye::update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects, ItemManager& items) {
    Enemy::update(dt, player,collisionRects, items);
    
    if (invincibleCooldownTimer <= 0 && projectile) {
        updateProjectiles(dt, player);
    }
}

void Eye::draw(sf::RenderTarget& target) {
    if (projectile) {
        projectile->draw(target);
    }
    
    Enemy::draw(target);
}