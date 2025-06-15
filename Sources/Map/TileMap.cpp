#include "TileMap.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

TileMap::TilesetInfo TileMap::getTilesetInfoForGid(int gid) const {
    auto it = m_gidStartToTileset.upper_bound(gid);
    if (it != m_gidStartToTileset.begin()) {
        --it;
    } else {
        return {"", -1};
    }
    return {it->second, it->first};
}

bool TileMap::load(const std::string& tmxPath, const std::vector<std::pair<std::string, std::string>>& tilesets) {
    m_layersVertices.clear();
    m_tilesetTextures.clear();

    for (const auto& [tilesetName, tilesetPath] : tilesets) {
        sf::Texture texture;
        if (!texture.loadFromFile(tilesetPath)) {
            std::cerr << "Failed to load tileset: " << tilesetPath << '\n';
            return false;
        }
        m_tilesetTextures[tilesetName] = std::move(texture);
    }


    pugi::xml_document doc;
    if (!doc.load_file(tmxPath.c_str())) {
        std::cerr << "Failed to load TMX file: " << tmxPath << '\n';
        return false;
    }

    pugi::xml_node mapNode = doc.child("map");
    if (!mapNode) return false;

    if (mapNode.attribute("infinite").as_bool()) {
        std::cerr << "Error: This loader only supports fixed-size maps. Please disable 'Infinite' in Tiled map properties." << '\n';
        return false;
    }

    mapWidth     = mapNode.attribute("width").as_uint();
    mapHeight    = mapNode.attribute("height").as_uint();
    tileWidth    = mapNode.attribute("tilewidth").as_uint();
    tileHeight   = mapNode.attribute("tileheight").as_uint();
    // std::map<int, std::string> gidStartToTileset;
    m_gidStartToTileset.clear();
    for (pugi::xml_node tilesetNode : mapNode.children("tileset")) {
        int firstGid = tilesetNode.attribute("firstgid").as_int();
        std::string source = tilesetNode.attribute("source").as_string(); 
        std::string name   = tilesetNode.attribute("name").as_string();  
        std::string tilesetName;
        if (!source.empty()) {
            size_t lastSlash = source.find_last_of("/\\");
            size_t start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
            size_t end = source.find_last_of('.');
            tilesetName = source.substr(start, end - start);
        } else {
            tilesetName = name;
        }
        m_gidStartToTileset[firstGid] = tilesetName;
    }

    // std::vector<int> gids;
    // for (const auto& [gid, _] : gidStartToTileset) gids.push_back(gid);
    // std::sort(gids.begin(), gids.end());

    // for (size_t i = 0; i < gids.size(); ++i) {
    //     int start = gids[i];
    //     constexpr int MAX_GID_PER_TILESET = 10000;
    //     int  end = (i + 1 < gids.size()) ? gids[i + 1] - 1 : gids[i] + MAX_GID_PER_TILESET;
    //     std::string tilesetName = gidStartToTileset[start];
    //     for (int gid = start; gid <= end; ++gid) {
    //         m_gidToTilesetName[gid] = tilesetName;
    //     }
    // }

    // --- THÊM MỚI: Đọc dữ liệu animation ---
    // std::cout << "Loading animation data..." << std::endl;

    // Lấy đường dẫn thư mục của file TMX để tìm file TSX
    std::string tmxDirectory;
    size_t lastSlash = tmxPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        tmxDirectory = tmxPath.substr(0, lastSlash + 1);
    }

    for (pugi::xml_node tilesetNodeInMap : doc.child("map").children("tileset")) {
        int firstGid = tilesetNodeInMap.attribute("firstgid").as_int();
        pugi::xml_attribute sourceAttr = tilesetNodeInMap.attribute("source");

        if (sourceAttr) {
            // --- XỬ LÝ TILESET NGOÀI (.tsx) ---
            std::string tsxPath = tmxDirectory + sourceAttr.as_string();
            
            pugi::xml_document tsxDoc;
            if (!tsxDoc.load_file(tsxPath.c_str())) {
                std::cerr << "Failed to load external tileset file: " << tsxPath << std::endl;
                continue;
            }
            
            // std::cout << "Successfully loaded external tileset: " << tsxPath << std::endl;

            pugi::xml_node tsxTilesetNode = tsxDoc.child("tileset");
            for (pugi::xml_node tileNode : tsxTilesetNode.children("tile")) {
                int localId = tileNode.attribute("id").as_int();
                int globalId = firstGid + localId;

                pugi::xml_node animationNode = tileNode.child("animation");
                if (animationNode) {
                    TileMapAnimation anim;
                    for (pugi::xml_node frameNode : animationNode.children("frame")) {
                        int frameLocalId = frameNode.attribute("tileid").as_int();
                        float durationMs = frameNode.attribute("duration").as_float();
                        
                        anim.frames.emplace_back(firstGid + frameLocalId, durationMs / 1000.f);
                    }
                    if (!anim.frames.empty()) {
                        // std::cout << "Found animation for GID: " << globalId << std::endl;
                        m_animations[globalId] = anim;
                    }
                }
            }

        } else {
            // --- XỬ LÝ TILESET NHÚNG TRỰC TIẾP (code cũ của bạn) ---
            for (pugi::xml_node tileNode : tilesetNodeInMap.children("tile")) {
                int localId = tileNode.attribute("id").as_int();
                int globalId = firstGid + localId;

                pugi::xml_node animationNode = tileNode.child("animation");
                if (animationNode) {
                    TileMapAnimation anim;
                    for (pugi::xml_node frameNode : animationNode.children("frame")) {
                        int frameLocalId = frameNode.attribute("tileid").as_int();
                        float durationMs = frameNode.attribute("duration").as_float();
                        
                        anim.frames.emplace_back(firstGid + frameLocalId, durationMs / 1000.f);
                    }
                    if (!anim.frames.empty()) {
                        // std::cout << "Found embedded animation for GID: " << globalId << std::endl;
                        m_animations[globalId] = anim;
                    }
                }
            }
        }
    }
    // std::cout << "Finished loading animation data. Total animations: " << m_animations.size() << std::endl;
    // --- KẾT THÚC THÊM MỚI ---

    for (pugi::xml_node layerNode : mapNode.children("layer")) {
        pugi::xml_node dataNode = layerNode.child("data");
        if (!dataNode || std::string(dataNode.attribute("encoding").as_string()) != "csv") {
            continue;
        }

        std::string csvData = dataNode.text().get();
        std::vector<int> layerTileIds;
        std::stringstream ss(csvData);
        std::string tileIdStr;

        layerTileIds.reserve(mapWidth * mapHeight);

        std::string line;
        while (std::getline(ss, line, ',')) {
            int id = std::atoi(line.c_str());
            layerTileIds.push_back(id);
        }
        
        if (layerTileIds.size() != mapWidth * mapHeight) {
            std::cerr << "Layer '" << layerNode.attribute("name").as_string() 
                      << "' data size does not match map dimensions!" << '\n';
            continue; 
        }

        // std::unordered_map<std::string, int> tilesetNameToFirstGid;
        // for (const auto& [gid, name] : gidStartToTileset) {
        //     tilesetNameToFirstGid[name] = gid;
        // }

        std::unordered_map<std::string, sf::VertexArray> tilesetToVertices;
        for (unsigned int i = 0; i < mapWidth; ++i) {
            for (unsigned int j = 0; j < mapHeight; ++j) {
                int tileGid = layerTileIds[i + j * mapWidth];
                if (tileGid == 0) continue;
                
                TilesetInfo tsInfo = getTilesetInfoForGid(tileGid);
                if (tsInfo.name.empty()) continue; // Bỏ qua nếu không tìm thấy tileset

                // --- SỬA ĐỔI LỚN BẮT ĐẦU TỪ ĐÂY ---
                // Kiểm tra xem tile này có animation không
                auto animIt = m_animations.find(tileGid);
                if (animIt != m_animations.end()) {
                    // ---- ĐÂY LÀ TILE ĐỘNG ----
                    
                    // Lấy thông tin tileset
                    // std::string tilesetName = m_gidToTilesetName[tileGid];
                    // const sf::Texture& tex = m_tilesetTextures.at(tilesetName);
                    const sf::Texture& tex = m_tilesetTextures.at(tsInfo.name);
                    // int firstGidForTileset = tilesetNameToFirstGid.at(tilesetName);

                    // Tạo một instance AnimatedTile mới
                    AnimatedTile animatedTile;
                    animatedTile.animationData = &animIt->second; // Trỏ tới dữ liệu animation
                    animatedTile.currentFrame = 0;
                    animatedTile.vertices.setPrimitiveType(sf::Quads);
                    
                    // Tạo 4 đỉnh cho tile này
                    sf::Vertex quad[4];
                    quad[0].position = sf::Vector2f(i * tileWidth, j * tileHeight);
                    quad[1].position = sf::Vector2f((i + 1) * tileWidth, j * tileHeight);
                    quad[2].position = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
                    quad[3].position = sf::Vector2f(i * tileWidth, (j + 1) * tileHeight);

                    // Set texCoords cho frame đầu tiên
                    int initialFrameTileGid = animatedTile.animationData->frames[0].tileID;
                    TilesetInfo frameTsInfo = getTilesetInfoForGid(initialFrameTileGid); 
                    int localTileId = initialFrameTileGid - frameTsInfo.firstGid;
                    int tilesPerRow = tex.getSize().x / tileWidth;
                    int tu = localTileId % tilesPerRow;
                    int tv = localTileId / tilesPerRow;
                    
                    quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
                    quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
                    quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
                    quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);

                    for(int k=0; k<4; ++k) animatedTile.vertices.append(quad[k]);

                    m_animatedTiles.push_back(std::move(animatedTile));
                } 
                else {
                    // ---- ĐÂY LÀ TILE TĨNH ----
                    // if (m_gidToTilesetName.count(tileGid) == 0) continue; 

                    // std::string tilesetName = m_gidToTilesetName[tileGid];
                    // auto texIt = m_tilesetTextures.find(tilesetName);
                    // if (texIt == m_tilesetTextures.end()) continue;
                    // const sf::Texture& tex = texIt->second;
                    const sf::Texture& tex = m_tilesetTextures.at(tsInfo.name);

                    // int firstGidForTileset = tilesetNameToFirstGid.at(tilesetName);

                    int localTileId = tileGid - tsInfo.firstGid;
                    int tilesPerRow = tex.getSize().x / tileWidth;
                    int tu = localTileId % tilesPerRow;
                    int tv = localTileId / tilesPerRow;

                    sf::VertexArray& va = tilesetToVertices[tsInfo.name];
                    if (va.getVertexCount() == 0) {
                        va.setPrimitiveType(sf::Quads);
                    }

                    sf::Vertex quad[4];

                    quad[0].position = sf::Vector2f(i * tileWidth, j * tileHeight);
                    quad[1].position = sf::Vector2f((i + 1) * tileWidth, j * tileHeight);
                    quad[2].position = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
                    quad[3].position = sf::Vector2f(i * tileWidth, (j + 1) * tileHeight);

                    quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
                    quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
                    quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
                    quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);

                    va.append(quad[0]);
                    va.append(quad[1]);
                    va.append(quad[2]);
                    va.append(quad[3]);
                }
                // --- KẾT THÚC SỬA ĐỔI ---
            }
        }
        for (auto& [tilesetName, va] : tilesetToVertices) {
            m_layerData.emplace_back(tilesetName, std::move(va));
        }
    }
    
    for (pugi::xml_node groupNode : mapNode.children("group")) {
        std::string groupName = groupNode.attribute("name").as_string();
        for (pugi::xml_node objectGroupNode : groupNode.children("objectgroup")) {
            std::string objectGroupName = objectGroupNode.attribute("name").as_string();
            for (pugi::xml_node objectNode : objectGroupNode.children("object")) {
                float x      = objectNode.attribute("x").as_float();
                float y      = objectNode.attribute("y").as_float();
                float width  = objectNode.attribute("width").as_float();
                float height = objectNode.attribute("height").as_float();
                
                if (groupName == "Collision") {
                    if (objectGroupName == "CollisionObjects") {
                        m_collisionRects.emplace_back(x, y, width, height);
                    }
                    else if (objectGroupName == "Npc") {
                        auto prop = objectNode.child("properties").find_child_by_attribute("property", "name", "npc_id");
                        int id = -1;
                        if (prop) {
                            id = prop.attribute("value").as_int();
                        }
                        m_NPCRects[id] = sf::FloatRect(x, y, width, height);
                    }
                    else if (objectGroupName == "Region") {
                        auto prop = objectNode.child("properties").find_child_by_attribute("property", "name", "region_id");
                        int id = -1;
                        if (prop) {
                            id = prop.attribute("value").as_int();
                        }
                        m_RegionRects[id] = sf::FloatRect(x, y, width, height);
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load() Npc\n";
                    }
                }
                else if (groupName == "Enemy") {
                    if (objectGroupName == "Bat Lv.1") {
                        m_enemyRects["Bat Lv.1"].emplace_back(x, y, width, height);
                    }
                    else if (objectGroupName == "Eye Lv.5") {
                        m_enemyRects["Eye Lv.5"].emplace_back(x, y, width, height);
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load() Enemy\n";
                    }
                }
                else {
                    std::cerr << "[Bug] - TileMap.cpp - load()\n";
                }
            }
        }
    }

    return true;
}

void TileMap::updateObjects() {
    for (sf::FloatRect& rect : m_collisionRects) {
        rect = getTransform().transformRect(rect);
    }
    
    for (auto& pair : m_enemyRects) {
        for (sf::FloatRect& rect : pair.second) {
            rect = getTransform().transformRect(rect);
        }
    }

    for (auto& pair : m_NPCRects) {
        pair.second = getTransform().transformRect(pair.second);
    }

    for (auto& pair : m_RegionRects) {
        pair.second = getTransform().transformRect(pair.second);
    }
}

sf::FloatRect TileMap::getGlobalBounds() const {
    // 1. Lấy kích thước gốc của map (kích thước cục bộ)
    float width  = static_cast<float>(mapWidth * tileWidth);
    float height = static_cast<float>(mapHeight * tileHeight);
    sf::FloatRect localBounds(0.f, 0.f, width, height);

    // 2. Lấy ma trận transform hiện tại của đối tượng
    const sf::Transform& transform = getTransform();

    // 3. Áp dụng ma trận transform lên khung chữ nhật cục bộ để có được khung bao toàn cục
    return transform.transformRect(localBounds);
}

sf::Vector2u TileMap::getPixelSize() const {
    // Lấy khung chữ nhật bao quanh toàn cục đã tính toán các phép biến đổi
    sf::FloatRect bounds = getGlobalBounds();

    // Trả về kích thước của khung đó
    return sf::Vector2u(
        static_cast<unsigned int>(bounds.width),
        static_cast<unsigned int>(bounds.height)
    );
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const auto& [tilesetName, layer] : m_layerData) {
        states.texture = &m_tilesetTextures.at(tilesetName);
        target.draw(layer, states);
    }

     // --- THÊM MỚI ---
    for (const auto& tile : m_animatedTiles) {
        // Tìm GID của frame hiện tại để xác định đúng texture
        int currentFrameGid = tile.animationData->frames[tile.currentFrame].tileID;
        TilesetInfo tsInfo = getTilesetInfoForGid(currentFrameGid);
        if (tsInfo.name.empty()) continue;

        // std::string tilesetName = m_gidToTilesetName.at(tsInfo.name);

        states.texture = &m_tilesetTextures.at(tsInfo.name);
        target.draw(tile.vertices, states);
    }
    // --- KẾT THÚC THÊM MỚI ---

    sf::RectangleShape hitbox;
    hitbox.setOutlineColor(sf::Color::Cyan);
    hitbox.setOutlineThickness(1.f);
    hitbox.setFillColor(sf::Color::Transparent);
    // for (const sf::FloatRect& rect : m_collisionRects) {
    //     hitbox.setSize(rect.getSize());
    //     hitbox.setPosition(rect.getPosition());
    //     target.draw(hitbox);
    // }
    
    hitbox.setOutlineColor(sf::Color::Yellow);
    for (auto& pair : m_enemyRects) {
        for (const sf::FloatRect& rect : pair.second) {
            hitbox.setSize(rect.getSize());
            hitbox.setPosition(rect.getPosition());
            target.draw(hitbox);
        }
    }

    hitbox.setOutlineColor(sf::Color::Green);
    for (auto& pair : m_NPCRects) {
        hitbox.setSize(pair.second.getSize());
        hitbox.setPosition(pair.second.getPosition());
        target.draw(hitbox);
    }

    hitbox.setOutlineColor(sf::Color::Magenta);
    for (auto& pair : m_RegionRects) {
        hitbox.setSize(pair.second.getSize());
        hitbox.setPosition(pair.second.getPosition());
        target.draw(hitbox);
    }
}

void TileMap::drawMinimap(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const auto& [tilesetName, layer] : m_layerData) {
        states.texture = &m_tilesetTextures.at(tilesetName);
        target.draw(layer, states);
    }

    for (const auto& tile : m_animatedTiles) {
        // Tìm GID của frame hiện tại để xác định đúng texture
        int currentFrameGid = tile.animationData->frames[tile.currentFrame].tileID;
        TilesetInfo tsInfo = getTilesetInfoForGid(currentFrameGid);
        if (tsInfo.name.empty()) continue;

        // std::string tilesetName = m_gidToTilesetName.at(tsInfo.name);

        states.texture = &m_tilesetTextures.at(tsInfo.name);
        target.draw(tile.vertices, states);
    }

    sf::RectangleShape hitbox;
    hitbox.setOutlineColor(sf::Color::Cyan);
    hitbox.setOutlineThickness(5.f);
    hitbox.setFillColor(sf::Color::Transparent);
    // for (const sf::FloatRect& rect : m_collisionRects) {
    //     hitbox.setSize(rect.getSize());
    //     hitbox.setPosition(rect.getPosition());
    //     target.draw(hitbox);
    // }
    
    // hitbox.setOutlineColor(sf::Color::Yellow);
    // for (auto& pair : m_enemyRects) {
    //     for (const sf::FloatRect& rect : pair.second) {
    //         hitbox.setSize(rect.getSize());
    //         hitbox.setPosition(rect.getPosition());
    //         target.draw(hitbox);
    //     }
    // }

    hitbox.setOutlineColor(sf::Color::Green);
    for (auto& pair : m_NPCRects) {
        hitbox.setSize(pair.second.getSize());
        hitbox.setPosition(pair.second.getPosition());
        target.draw(hitbox);
    }

    hitbox.setOutlineColor(sf::Color::Magenta);
    for (auto& pair : m_RegionRects) {
        hitbox.setSize(pair.second.getSize());
        hitbox.setPosition(pair.second.getPosition());
        target.draw(hitbox);
    }
}

const std::vector<sf::FloatRect>& TileMap::getCollisionRects() const {
    return m_collisionRects;
}

const std::unordered_map<int, sf::FloatRect>& TileMap::getNpcRects() const {
    return m_NPCRects;
}

const std::unordered_map<std::string, std::vector<sf::FloatRect>>& TileMap::getEnemyRects() const {
    return m_enemyRects;
}

const std::unordered_map<int, sf::FloatRect>& TileMap::getRegionRects() const {
    return m_RegionRects;
}

void TileMap::update(const float& dt) {
    for (auto& tile : m_animatedTiles) {
        tile.elapsedTime += dt;
        
        const auto& frames = tile.animationData->frames;
        if (tile.elapsedTime >= frames[tile.currentFrame].duration) {
            // Trừ đi thời gian của frame đã qua
            tile.elapsedTime -= frames[tile.currentFrame].duration;
            
            // Chuyển sang frame tiếp theo, quay vòng lại nếu hết
            tile.currentFrame = (tile.currentFrame + 1) % frames.size();

            // Cập nhật texture coordinates cho frame mới
            int nextFrameGid = frames[tile.currentFrame].tileID;
            
            TilesetInfo tsInfo = getTilesetInfoForGid(nextFrameGid);
            if (tsInfo.name.empty()) continue; // Bỏ qua nếu có lỗi

            // Cần có cách map GID -> Tên tileset hiệu quả
            // m_gidToTilesetName đã làm việc này
            const sf::Texture& tex = m_tilesetTextures.at(tsInfo.name);

            // Cần có cách map Tên tileset -> firstGid hiệu quả
            int localTileId = nextFrameGid - tsInfo.firstGid;
            int tilesPerRow = tex.getSize().x / tileWidth;
            int tu = localTileId % tilesPerRow;
            int tv = localTileId / tilesPerRow;

            // Cập nhật texCoords cho 4 đỉnh của tile
            tile.vertices[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
            tile.vertices[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
            tile.vertices[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
            tile.vertices[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);
        }
    }
}