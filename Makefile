# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

# SFML flags (adjust if SFML is in a custom path)
SFML_LIBS = -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio

INCLUDES = -I"./Sources" \
		   -I"./Sources/AnimationManager" \
		   -I"./Sources/BossAltar" \
		   -I"./Sources/BossAltarManager" \
		   -I"./Sources/Clock" \
		   -I"./Sources/Configuration" \
		   -I"./Sources/Effects" \
		   -I"./Sources/EnemyManager" \
		   -I"./Sources/Entities" \
		   -I"./Sources/Entities/Enemies" \
		   -I"./Sources/Entities/Enemies/Bat" \
		   -I"./Sources/Entities/Enemies/BatBoss" \
		   -I"./Sources/Entities/Enemies/Eye" \
		   -I"./Sources/Entities/Enemies/Slime" \
		   -I"./Sources/Entities/Enemies/VampireBat" \
		   -I"./Sources/Entities/Item" \
		   -I"./Sources/Entities/Npc" \
		   -I"./Sources/Entities/Player" \
		   -I"./Sources/Entities/Projectile" \
		   -I"./Sources/FileSystem" \
		   -I"./Sources/Inventory" \
		   -I"./Sources/ItemDatabase" \
		   -I"./Sources/ItemManager" \
		   -I"./Sources/Items" \
		   -I"./Sources/LightSource" \
		   -I"./Sources/Maps" \
		   -I"./Sources/NpcManager" \
		   -I"./Sources/ParticleManager" \
		   -I"./Sources/QuestManager" \
		   -I"./Sources/Quests" \
		   -I"./Sources/Quests/QuestObjectives" \
		   -I"./Sources/SoundManager" \
		   -I"./Sources/TextureManager" \
		   -I"./Sources/UI"

# Directories
SRC_DIR   = Sources
BUILD_DIR = Build
TARGET    = ./Build/Demo.exe

# Find all .cpp files in SRC_DIR
SRCS := $(wildcard $(SRC_DIR)/*.cpp) \
        $(wildcard $(SRC_DIR)/AnimationManager/*.cpp) \
        $(wildcard $(SRC_DIR)/BossAltar/*.cpp) \
        $(wildcard $(SRC_DIR)/BossAltarManager/*.cpp) \
        $(wildcard $(SRC_DIR)/Clock/*.cpp) \
        $(wildcard $(SRC_DIR)/Configuration/*.cpp) \
        $(wildcard $(SRC_DIR)/Effects/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/*.cpp) \
        $(wildcard $(SRC_DIR)/EnemyManager/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Enemies/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Enemies/Bat/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Enemies/BatBoss/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Enemies/Eye/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Enemies/Slime/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Enemies/VampireBat/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Item/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Npc/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Player/*.cpp) \
        $(wildcard $(SRC_DIR)/Entities/Projectile/*.cpp) \
        $(wildcard $(SRC_DIR)/FileSystem/*.cpp) \
        $(wildcard $(SRC_DIR)/Inventory/*.cpp) \
        $(wildcard $(SRC_DIR)/ItemDatabase/*.cpp) \
        $(wildcard $(SRC_DIR)/ItemManager/*.cpp) \
        $(wildcard $(SRC_DIR)/Items/*.cpp) \
        $(wildcard $(SRC_DIR)/LightSource/*.cpp) \
        $(wildcard $(SRC_DIR)/Maps/*.cpp) \
        $(wildcard $(SRC_DIR)/NpcManager/*.cpp) \
        $(wildcard $(SRC_DIR)/ParticleManager/*.cpp) \
        $(wildcard $(SRC_DIR)/QuestManager/*.cpp) \
        $(wildcard $(SRC_DIR)/Quests/*.cpp) \
        $(wildcard $(SRC_DIR)/Quests/QuestObjectives/*.cpp) \
        $(wildcard $(SRC_DIR)/SoundManager/*.cpp) \
        $(wildcard $(SRC_DIR)/TextureManager/*.cpp) \
        $(wildcard $(SRC_DIR)/UI/*.cpp) 

# Generate corresponding .o filenames
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files into binary
$(TARGET): $(OBJS)
	@echo Linking...
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS) $(INCLUDES)

# Compile each .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Compiling: $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MMD -c $< -o $@ $(INCLUDES)

# Run the program
run: $(TARGET)
	$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Include dependency files
DEPS := $(OBJS:.o=.d)
-include $(DEPS)
