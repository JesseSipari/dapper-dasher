// game.hpp
#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"
#include "TextureWrapper.hpp"
#include <string>

// Animation Data Structure
struct AnimData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

// Game Assets Structure using RAII Wrappers
struct GameAssets {
    // Background Layers
    TextureWrapper background1;
    TextureWrapper background2;
    float background1_x;
    float background2_x;

    // Midground Layers
    TextureWrapper midground1;
    TextureWrapper midground2;
    float midground1_x;
    float midground2_x;

    // Foreground Layers
    TextureWrapper foreground1;
    TextureWrapper foreground2;
    float foreground1_x;
    float foreground2_x;

    // Other Assets
    TextureWrapper scarfy;
    TextureWrapper nebula;

    // Constructor initializes all textures and layer positions
    GameAssets(const std::string& backgroundPath, const std::string& midgroundPath, const std::string& foregroundPath,
        const std::string& scarfyPath, const std::string& nebulaPath)
        : background1(backgroundPath), background2(backgroundPath),
        midground1(midgroundPath), midground2(midgroundPath),
        foreground1(foregroundPath), foreground2(foregroundPath),
        scarfy(scarfyPath), nebula(nebulaPath),
        background1_x(0.0f),
        background2_x(static_cast<float>(background1.get().width) * 2.0f),
        midground1_x(0.0f),
        midground2_x(static_cast<float>(midground1.get().width) * 2.0f),
        foreground1_x(0.0f),
        foreground2_x(static_cast<float>(foreground1.get().width) * 2.0f) {}
};

//
// Function Declarations
//
void InitializeGame(int windowDimensions[2]);
void UpdatePlayerAnimation(AnimData& data, const float deltaTime, const int windowDimensions[2]);
void UpdatePlayer(AnimData& data, float& velocity, const float gravity, const float deltaTime, const float jumpVelocity, const int windowDimensions[2]);
void UpdateNebulaAnimation(AnimData& data, const float deltaTime, const int sizeOfNebulae);
void UpdateAllNebulae(AnimData nebulae[], const int sizeOfNebulae, const float nebulaVelocity, const float deltaTime);
void DrawGame(const GameAssets& assets, const AnimData& scarfyData, const AnimData nebulae[], int nebulaCount, bool collision);
bool isOnGround(const AnimData data, const int windowDimensions[2]);
void UpdateFinishLine(float& finishLine, const AnimData nebulae[], int sizeOfNebulae);
#endif // GAME_HPP
