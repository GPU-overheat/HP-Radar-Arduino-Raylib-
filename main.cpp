#include "raylib.h"
#include "serialib.h"
#include <iostream>
#include <vector>
#include <cmath>

struct Ping {
    Vector2 pos;
    float alpha;
};

int main() {
    serialib serial;
    if (serial.openDevice("/dev/ttyACM0", 9600) != 1) {
        printf("couldn't open device!\n");
        return 1;
    }

    InitWindow(800, 600, "Raylib Radar");
    SetTargetFPS(60);

    Vector2 origin = { 400.0f, 550.0f }; 
    float scale = 3.5f;            
    std::vector<Ping> history;
    
    char buffer[32];
    int angle = 0;
    int distance = 0;

    while (!WindowShouldClose()) {
        if (serial.readString(buffer, '\n', 32, 2) > 0) {
            if (sscanf(buffer, "%d,%d", &angle, &distance) == 2) {
                float rads = (float)angle * (PI / 180.0f);
                Vector2 worldPos = {
                    origin.x + (cosf(rads) * (float)distance * scale),
                    origin.y - (sinf(rads) * (float)distance * scale) 
                };
                
                if (distance > 0 && distance < 200) {
                    history.push_back({ worldPos, 1.0f });
                }
            }
        }
        for (size_t i = 0; i < history.size(); i++) {
            history[i].alpha -= 0.01f;
            if (history[i].alpha <= 0) {
                history.erase(history.begin() + (int)i);
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);

            for (int r = 1; r <= 4; r++) {
                DrawCircleLinesV(origin, (float)r * 50.0f * scale, DARKGREEN);
            }
            for (auto& p : history) {
                DrawCircleV(p.pos, 3.0f, Fade(LIME, p.alpha));
            }

            float beamRads = (float)angle * (PI / 180.0f);
            Vector2 beamEnd = {
                origin.x + (cosf(beamRads) * 200.0f * scale),
                origin.y - (sinf(beamRads) * 200.0f * scale)
            };
            DrawLineV(origin, beamEnd, GREEN);

            DrawText(TextFormat("Angle: %d", angle), 10, 10, 20, GREEN);
            DrawText(TextFormat("Dist: %d cm", distance), 10, 40, 20, GREEN);
        EndDrawing();
    }

    serial.closeDevice();
    CloseWindow();
    return 0;
}