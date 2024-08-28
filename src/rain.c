#include "../include/world/rain.h"

// rain 

#define RAINDROP_COUNT 1000
#define RAINDROP_SCALE (Vector3){1.0f, 1.0f, 1.0f}


typedef struct RainDrop {
    Vector3 position;
    float rotationY;
} rain_drop;

rain_drop rainDrops[RAINDROP_COUNT];

Model Rain() {
    Mesh plane = GenMeshPlane(0.03f, 0.3f, 1, 1);
    Model rain = LoadModelFromMesh(plane);
    return rain;
}

void InitRain() {
    for (int i = 0; i < RAINDROP_COUNT; i++) {
        rainDrops[i].position.x = GetRandomValue(-20, 20);
        rainDrops[i].position.y = GetRandomValue(5, 20);
        rainDrops[i].position.z = GetRandomValue(-20, 20);
    }
}

void UpdateRain() {
    for (int i = 0; i < RAINDROP_COUNT; i++) {
        rainDrops[i].position.y -= 0.1f;
        if (rainDrops[i].position.y < 0) {
            rainDrops[i].position.y = GetRandomValue(5, 20);
        }
    }
}

void DrawRain(Model rain) {
    for (int i = 0; i < RAINDROP_COUNT; i++) {
        DrawModelEx(rain, rainDrops[i].position, (Vector3){1.0f, 0.0f, 0.0f}, 90.0f, RAINDROP_SCALE, DARKGRAY);
    }
}