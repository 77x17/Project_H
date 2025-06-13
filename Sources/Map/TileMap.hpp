#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "pugixml.hpp"

class TileMap : public sf::Drawable, public sf::Transformable {
private:
    unsigned int mapWidth;
    unsigned int mapHeight;
    unsigned int tileWidth;
    unsigned int tileHeight;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<sf::VertexArray> m_layersVertices; 
    sf::Texture m_tilesetTexture;

    std::vector<sf::FloatRect> m_collisionRects;
    std::unordered_map<int, sf::FloatRect> m_NPCRects;
    std::unordered_map<std::string, std::vector<sf::FloatRect>> m_enemyRects;

public:
    bool load(const std::string& tmxPath, const std::string& tilesetPath);

    void updateObjects();

    sf::Vector2u getPixelSize() const;
    void drawMinimap(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    const std::vector<sf::FloatRect>& getCollisionRects() const;
    const std::unordered_map<int, sf::FloatRect>& getNpcRects() const;
    const std::unordered_map<std::string, std::vector<sf::FloatRect>>& getEnemyRects() const;
};