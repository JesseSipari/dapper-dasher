// dasher.cpp

#include "game.hpp"
#include "raylib.h"
#include <iostream>

int main() {
    try {
        // Window dimensions
        int windowDimensions[2] = { 512, 380 };
        const float gravity = 1000.0f; // Acceleration due to gravity (pixels/s^2)
        const float nebulaVelocity = -400.0f; // Nebula X velocity (pixels/s)
        const float jumpVelocity = -600.0f; // Jump velocity (pixels/s)
        float velocity = 0.0f; // Player's vertical velocity
        bool collision{}; // Collision flag


        // Initialize raylib window
        InitializeGame(windowDimensions);

        // Initialize Game Assets with the same background texture for seamless scrolling
        GameAssets assets("textures/far-buildings.png", "textures/back-buildings.png", "textures/foreground.png", "textures/scarfy.png", "textures/12_nebula_spritesheet.png");

        // Scarfy Animation Data
        AnimData scarfyData = {
            Rectangle{0.0f, 0.0f, static_cast<float>(assets.scarfy.get().width) / 6.0f, static_cast<float>(assets.scarfy.get().height)},
            Vector2{static_cast<float>(windowDimensions[0] / 2) - (assets.scarfy.get().width / 12.0f), static_cast<float>(windowDimensions[1]) - assets.scarfy.get().height},
            0,              // frame
            1.0f / 12.0f,   // updateTime
            0.0f            // runningTime
        };

        // Nebulae Setup
        const int sizeOfNebulae = 6;
        AnimData nebulae[sizeOfNebulae];

        for (int i = 0; i < sizeOfNebulae; i++) {
            nebulae[i].rec = Rectangle{ 0.0f, 0.0f, static_cast<float>(assets.nebula.get().width) / 8.0f, static_cast<float>(assets.nebula.get().height) / 8.0f };
            nebulae[i].pos = Vector2{ static_cast<float>(windowDimensions[0]) + i * 300.0f, static_cast<float>(windowDimensions[1]) - nebulae[i].rec.height };
            nebulae[i].frame = 0;
            nebulae[i].updateTime = 1.0f / 16.0f;
            nebulae[i].runningTime = 0.0f;
        }


		
        float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

        
        // Main Game Loop
        while (!WindowShouldClose()) {
            float deltaTime = GetFrameTime();

            // Update layer positions
            assets.background1_x -= 10.0f * deltaTime; // Background speed
            assets.background2_x -= 10.0f * deltaTime;
            assets.midground1_x -= 15.0f * deltaTime;  // Midground speed
            assets.midground2_x -= 15.0f * deltaTime;
            assets.foreground1_x -= 60.0f * deltaTime; // Foreground speed
            assets.foreground2_x -= 60.0f * deltaTime;

            // Reset positions for seamless scrolling
            if (assets.background1_x <= -assets.background1.get().width * 2) {
                assets.background1_x = assets.background2_x + assets.background2.get().width * 2;
            }
            if (assets.background2_x <= -assets.background2.get().width * 2) {
                assets.background2_x = assets.background1_x + assets.background1.get().width * 2;
            }

            if (assets.midground1_x <= -assets.midground1.get().width * 2) {
                assets.midground1_x = assets.midground2_x + assets.midground2.get().width * 2;
            }
            if (assets.midground2_x <= -assets.midground2.get().width * 2) {
                assets.midground2_x = assets.midground1_x + assets.midground1.get().width * 2;
            }

            if (assets.foreground1_x <= -assets.foreground1.get().width * 2) {
                assets.foreground1_x = assets.foreground2_x + assets.foreground2.get().width * 2;
            }
            if (assets.foreground2_x <= -assets.foreground2.get().width * 2) {
                assets.foreground2_x = assets.foreground1_x + assets.foreground1.get().width * 2;
            }

            // Update game logic only if no collision has occurred
            if (!collision) {
                UpdateAllNebulae(nebulae, sizeOfNebulae, nebulaVelocity, deltaTime);
                UpdatePlayer(scarfyData, velocity, gravity, deltaTime, jumpVelocity, windowDimensions);
                UpdatePlayerAnimation(scarfyData, deltaTime, windowDimensions);
                UpdateFinishLine(finishLine, nebulae, sizeOfNebulae);
            }

            // Check for collisions
            collision = false;
            for (AnimData& nebula : nebulae) {
                float padding{ 50.0f };
                Rectangle nebulaRect{
                    nebula.pos.x + padding,
                    nebula.pos.y + padding,
                    nebula.rec.width - 2 * padding,
                    nebula.rec.height - 2 * padding };
                Rectangle scarfyRect{
                    scarfyData.pos.x,
                    scarfyData.pos.y,
                    scarfyData.rec.width,
                    scarfyData.rec.height };
                if (CheckCollisionRecs(nebulaRect, scarfyRect)) {
                    collision = true;
                    break;
                }
            }

            // Render game with collision state
            DrawGame(assets, scarfyData, nebulae, sizeOfNebulae, collision);
        }

        CloseWindow();
        return 0;
    }
    catch (const std::exception& e) { // <-- Added catch block
        std::cerr << "ERROR: " << e.what() << std::endl;
        CloseWindow(); // Ensure Raylib window is closed properly
        return EXIT_FAILURE;
    }
}
