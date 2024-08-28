#include "../include/world/rain.h"

// rain 

#define RAINDROP_COUNT 1000
#define RAINDROP_SCALE (Vector3){1.0f, 1.0f, 1.0f}
#define MAX_OFFSET 30.0f


typedef struct RainDrop {
    Vector3 position;
} rain_drop;

rain_drop rainDrops[RAINDROP_COUNT];

Model Rain() {
    Mesh plane = GenMeshPlane(0.03f, 0.3f, 1, 1);
    Model rain = LoadModelFromMesh(plane);

    return rain;
}

// Helper function to wrap rain drops position around the camera
static inline void WrapRainPosition(float *position, float min, float max) {
    if (*position > max) {
        *position = min;
    } else if (*position < min) {
        *position = max;
    }
}

// Initialize rain drops positions
void InitRain(Vector3 cameraPosition) {
    for (int i = 0; i < RAINDROP_COUNT; i++) {
        rainDrops[i].position.x = GetRandomValue(cameraPosition.x - MAX_OFFSET, cameraPosition.x + MAX_OFFSET);
        rainDrops[i].position.y = GetRandomValue(5, MAX_OFFSET);
        rainDrops[i].position.z = GetRandomValue(cameraPosition.z - MAX_OFFSET, cameraPosition.z + MAX_OFFSET);
    }
}
 
// Update rain drops position and wrap around the camera
void UpdateRain(Vector3 cameraPosition, int rainCount) {
    float maxX = cameraPosition.x + MAX_OFFSET;
    float minX = cameraPosition.x - MAX_OFFSET;
    float maxZ = cameraPosition.z + MAX_OFFSET;
    float minZ = cameraPosition.z - MAX_OFFSET;

    for (int i = 0; i < RAINDROP_COUNT; i++) {
        WrapRainPosition(&rainDrops[i].position.x, minX, maxX);
        WrapRainPosition(&rainDrops[i].position.z, minZ, maxZ);

        rainDrops[i].position.y -= 0.3f;

        if (rainDrops[i].position.y < 0) {
            rainDrops[i].position.y = GetRandomValue(20, MAX_OFFSET);
        }
    }


}

void DrawRain(Model rain, Vector3 cameraPosition, int rainCount) {
    UpdateRain(cameraPosition, rainCount);

    for (int i = 0; i < RAINDROP_COUNT; i++) {
        DrawModelEx(rain, rainDrops[i].position, (Vector3){1.0f, 0.0f, 0.0f}, 90.0f, RAINDROP_SCALE, DARKGRAY);
    }
}