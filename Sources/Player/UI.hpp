#pragma once

#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "TileMap.hpp"

class UI {
private:
    sf::Vector2f       PADDING;
    float              BOX_OUTLINE_THICKNESS;

    float              LEVEL_TEXT_SIZE;
    sf::Vector2f       LEVEL_BACKGROUND_SIZE;
    sf::Vector2f       LEVEL_POSISION;
    sf::Text           levelText;
    sf::RectangleShape levelBackground;

    float              HEALTH_POINTS_TEXT_SIZE;
    sf::Vector2f       HEALTH_BAR_SIZE;
    sf::Vector2f       HEALTH_BAR_POSITION;
    float              HEALTH_BAR_LEAP_SPEED;
    float              healthBarRatio;
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBackground;
    sf::Text           healthPointsText;
    
    float              XP_TEXT_SIZE;
    sf::Vector2f       XP_BAR_SIZE;
    sf::Vector2f       XP_BAR_POSISION;
    float              XP_BAR_LEAP_SPEED;
    float              XPProgress;
    sf::RectangleShape XPBar;
    sf::RectangleShape XPBarBackground;
    sf::Text           XPText;

    float              QUESTS_TEXT_SIZE;
    sf::Vector2f       QUESTS_PADDING;
    float              QUESTS_BOX_LEAP_SPEED;
    sf::Vector2f       questsPosition;
    sf::Vector2f       questsBoxSize;
    sf::Text           questsBoxLabel;
    sf::Text           questsText;
    sf::RectangleShape questsBox;
    bool               questsBoxVisible;

    sf::Vector2f       MINIMAP_PADDING;
    float              MINIMAP_LEAP_SPEED;
    sf::Vector2f       MINIMAP_SIZE;
    sf::Vector2f       minimapSize;
    sf::Vector2u       lastMinimapTextureSize;
    float              minimapViewRatio;
    sf::Vector2f       minimapPosition;
    sf::Vector2f       lastMinimapPosition;
    sf::RenderTexture  minimapTexture;
    sf::Sprite         minimapSprite;
    sf::View           minimapView;
    sf::RectangleShape minimapBox;
    bool               fullMinimapActive;
public:
    UI();

    void updateHealthBar(const float& dt, const Player& player);
    void updateLevelAndXP(const float& dt, const Player& player);
    void updateQuests(const float& dt, const std::vector<Quest>& quests, const sf::Vector2f& uiSize);
    void updateMinimap(const float& dt, const Player& player, const sf::Vector2f& uiSize, const TileMap& map);
    void update(const float& dt, Player& player, const sf::Vector2f& uiSize, const TileMap& map);

    void draw(sf::RenderWindow& window);

    void updateQuestsBox();

    // void generateMinimapTexture(const TileMap &map);
    void updateMinimapBoxSize(bool minimize);
    void openMap(const sf::Vector2f& uiSize);

};