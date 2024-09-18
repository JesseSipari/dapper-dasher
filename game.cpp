// game.cpp

#include "game.hpp"
#include "raylib.h"
#include <iostream>

// Initialize Raylib
void InitializeGame(int windowDimensions[2]) {
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");
    SetTargetFPS(60);
}

// Update Player Animation
void UpdatePlayerAnimation(AnimData& data, const float deltaTime, const int windowDimensions[2]) {
    if (isOnGround(data, windowDimensions)) {
        data.runningTime += deltaTime;
        if (data.runningTime >= data.updateTime) {
            data.runningTime = 0.0f;
            data.rec.x = data.frame * data.rec.width;
            data.frame++;
            if (data.frame > 5) { // Assuming 6 frames
                data.frame = 0;
            }
        }
    }
}

// Update Player Physics and Input
void UpdatePlayer(AnimData& data, float& velocity, const float gravity, const float deltaTime, const float jumpVelocity, const int windowDimensions[2]) {
    if (isOnGround(data, windowDimensions)) {
        velocity = 0.0f;
        data.pos.y = static_cast<float>(windowDimensions[1]) - data.rec.height;

        if (IsKeyPressed(KEY_SPACE)) {
            velocity = jumpVelocity;
        }
    }
    else {
        velocity += gravity * deltaTime;
    }

    data.pos.y += velocity * deltaTime;
}

// Update Nebula Animation (Simplified)
void UpdateNebulaAnimation(AnimData& data, const float deltaTime, const int sizeOfNebulae) {
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime) {
        data.runningTime = 0.0f;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > 7) { // Assuming 8 frames
            data.frame = 0;
        }
    }
}

// Update All Nebulae Positions and Animations
void UpdateAllNebulae(AnimData nebulae[], const int sizeOfNebulae, const float nebulaVelocity, const float deltaTime) {
    for (int i = 0; i < sizeOfNebulae; i++) {
        nebulae[i].pos.x += nebulaVelocity * deltaTime;
        UpdateNebulaAnimation(nebulae[i], deltaTime, sizeOfNebulae);
    }
}

// Update Finish Line
void UpdateFinishLine(float& finishLine, const AnimData* nebulae, int sizeOfNebulae) {
    if (sizeOfNebulae > 0) {
        // Example logic: Set finishLine to the x-position of the last nebula
        finishLine = nebulae[sizeOfNebulae - 1].pos.x;
    }
    // Additional logic can be added here as per game design
}

// Draw Game Objects
void DrawGame(const GameAssets& assets, const AnimData& scarfyData, const AnimData nebulae[], int nebulaCount, bool collision, const int windowDimensions[2]) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    float scale = 2.0f; // Define scaling factor

    // Draw Backgrounds
    Vector2 backgroundPos1{ assets.background1_x, 0.0f };
    Vector2 backgroundPos2{ assets.background2_x, 0.0f };
    DrawTextureEx(assets.background1.get(), backgroundPos1, 0.0f, scale, WHITE);
    DrawTextureEx(assets.background2.get(), backgroundPos2, 0.0f, scale, WHITE);

    // Draw Midgrounds
    Vector2 midgroundPos1{ assets.midground1_x, 0.0f };
    Vector2 midgroundPos2{ assets.midground2_x, 0.0f };
    DrawTextureEx(assets.midground1.get(), midgroundPos1, 0.0f, scale, WHITE);
    DrawTextureEx(assets.midground2.get(), midgroundPos2, 0.0f, scale, WHITE);

    // Draw Foregrounds
    Vector2 foregroundPos1{ assets.foreground1_x, 0.0f };
    Vector2 foregroundPos2{ assets.foreground2_x, 0.0f };
    DrawTextureEx(assets.foreground1.get(), foregroundPos1, 0.0f, scale, WHITE);
    DrawTextureEx(assets.foreground2.get(), foregroundPos2, 0.0f, scale, WHITE);

    // Conditionally Draw Nebulae and Scarfy
    if (!collision) {
        // Draw Nebulae
        for (int i = 0; i < nebulaCount; i++) {
            Color nebulaColor = (i % 2 == 0) ? WHITE : RED;
            DrawTextureRec(assets.nebula.get(), nebulae[i].rec, nebulae[i].pos, nebulaColor);
        }

        // Draw Scarfy
        DrawTextureRec(assets.scarfy.get(), scarfyData.rec, scarfyData.pos, WHITE);
    }
    else {
        // Draw Game Over Message
        const char* gameOverText = "Game Over!";
        int fontSize = 40;
        int textWidth = MeasureText(gameOverText, fontSize);
        int textX = (windowDimensions[0] / 2) - (textWidth / 2);
        int textY = (windowDimensions[1] / 2) - (fontSize / 2);
        DrawText(gameOverText, textX, textY, fontSize, RED);

        // Draw Restart Prompt
        const char* restartText = "Press R to Restart";
        int restartFontSize = 20;
        int restartTextWidth = MeasureText(restartText, restartFontSize);
        int restartTextX = (windowDimensions[0] / 2) - (restartTextWidth / 2);
        int restartTextY = textY + fontSize + 10;
        DrawText(restartText, restartTextX, restartTextY, restartFontSize, BLACK);
    }

    EndDrawing();
}

// Check if the Player is on the Ground
bool isOnGround(const AnimData data, const int windowDimensions[2]) {
    return data.pos.y >= static_cast<float>(windowDimensions[1]) - data.rec.height;
}
