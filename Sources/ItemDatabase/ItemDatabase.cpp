#include "ItemDatabase.hpp"

#include "AllItems.hpp"

#include <iostream>

std::unordered_map<std::string, std::unique_ptr<ItemData>> ItemDatabase::itemStorage;

void ItemDatabase::loadItems() {
    // --- [Begin] - Weapon ---
    itemStorage["Old Bow"]    = std::make_unique<Bow>("Old Bow"   , "bow_00", 1, ItemRarity::Normal , 1.0f);
    itemStorage["Wooden Bow"] = std::make_unique<Bow>("Wooden Bow", "bow_00", 3, ItemRarity::Unique , 2.5f);
    itemStorage["Bat Bow"]    = std::make_unique<Bow>("Bat Bow"   , "bow_00", 5, ItemRarity::Rare   , 5.0f);
    // --- [End] ---
    
    // --- [Begin] - Armor ---
    itemStorage["Old Helmet"]     = std::make_unique<Helmet>    ("Old Helmet"    , "helmet_00"    , 1, ItemRarity::Normal, 2.0f);
    itemStorage["Old Chestplate"] = std::make_unique<Chestplate>("Old Chestplate", "chestplate_00", 1, ItemRarity::Normal, 2.0f);
    itemStorage["Old Leggings"]   = std::make_unique<Leggings>  ("Old Leggings"  , "leggings_00"  , 1, ItemRarity::Normal, 2.0f);
    itemStorage["Old Boots"]      = std::make_unique<Boots>     ("Old Boots"     , "boots_00"     , 1, ItemRarity::Normal, 2.0f);
    
    itemStorage["Wooden Helmet"]     = std::make_unique<Helmet>    ("Wooden Helmet"    , "helmet_00"    , 3, ItemRarity::Unique, 3.0f);
    itemStorage["Wooden Chestplate"] = std::make_unique<Chestplate>("Wooden Chestplate", "chestplate_00", 3, ItemRarity::Unique, 3.0f);
    itemStorage["Wooden Leggings"]   = std::make_unique<Leggings>  ("Wooden Leggings"  , "leggings_00"  , 3, ItemRarity::Unique, 3.0f);
    itemStorage["Wooden Boots"]      = std::make_unique<Boots>     ("Wooden Boots"     , "boots_00"     , 3, ItemRarity::Unique, 3.0f);

    itemStorage["Bat Helmet"]     = std::make_unique<Helmet>    ("Bat Helmet"    , "helmet_00"    , 5, ItemRarity::Rare, 5.0f);
    itemStorage["Bat Chestplate"] = std::make_unique<Chestplate>("Bat Chestplate", "chestplate_00", 5, ItemRarity::Rare, 5.0f);
    itemStorage["Bat Leggings"]   = std::make_unique<Leggings>  ("Bat Leggings"  , "leggings_00"  , 5, ItemRarity::Rare, 5.0f);
    itemStorage["Bat Boots"]      = std::make_unique<Boots>     ("Bat Boots"     , "boots_00"     , 5, ItemRarity::Rare, 5.0f);
    // --- [End] ---

    // --- [Begin] - Material ---
    itemStorage["Bat Orb"] = std::make_unique<Orb>("Bat Orb", "orb", ItemRarity::Rare);
    itemStorage["Eye Orb"] = std::make_unique<Orb>("Eye Orb", "orb", ItemRarity::Rare);
    // --- [End] ---

    itemStorage["God Bow"] = std::make_unique<Bow>("God Bow", "bow_00", 1, ItemRarity::Mythic, 100.0f);
    itemStorage["God Helmet"] = std::make_unique<Helmet>("God Helmet", "helmet_00", 1, ItemRarity::Mythic, 100.0f);
}

std::shared_ptr<ItemData> ItemDatabase::get(const std::string& name, const int& amount) {
    if (name == std::string()) {
        return nullptr;
    }

    auto it = itemStorage.find(name);
    if (it != itemStorage.end()) {
        if (amount == 1) {
            return it->second->clone();  // trả về shared_ptr
        } 
        else if (amount >= 2) {
            auto clonedItem = it->second->clone();
            clonedItem->amount = amount; // dùng setter nếu có
            return clonedItem;
        }
    } 
        
    std::cerr << "Item not found in ItemDatabase: " << name << "\n";
    return nullptr;
}