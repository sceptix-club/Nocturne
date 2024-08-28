#include "../include/world/rain.h"

#define RAINDROP_COUNT 1000
#define RAINDROP_SCALE (Vector3){1.0f, 1.0f, 1.0f}
#define MAX_OFFSET 30.0f
#define START_RAIN_COUNT 0

typedef struct RainDrop {
    Vector3 position;
} rain_drop;

rain_drop rainDrops[RAINDROP_COUNT];

int activeRain = START_RAIN_COUNT;

Model Rain() {
    Mesh plane = GenMeshPlane(0.03f, 0.3f, 1, 1);
    Model rain = LoadModelFromMesh(plane);

    return rain;
}

inline void ResetActiveRainDrops()
{
    activeRain = START_RAIN_COUNT;
}

// Helper function to wrap rain drops position around the camera
static inline void WrapRainPosition(float* position, float min, float max) {
    if (*position > max) {
        *position = min;
    } else if (*position < min) {
        *position = max;
    }
}

static inline void CalculateMinMax(Vector3* cameraPosition, float* minX, float* maxX, float* minZ, float* maxZ) {
    *minX = cameraPosition->x - MAX_OFFSET;
    *maxX = cameraPosition->x + MAX_OFFSET;
    *minZ = cameraPosition->z - MAX_OFFSET;
    *maxZ = cameraPosition->z + MAX_OFFSET;
}

// Initialize rain drops positions
void InitRain(Vector3 cameraPosition) {
    float maxX, minX, maxZ, minZ;
    CalculateMinMax(&cameraPosition, &minX, &maxX, &minZ, &maxZ);

    for (int i = 0; i < RAINDROP_COUNT; i++) {
        rainDrops[i].position.x = GetRandomValue(minX, maxX);
        rainDrops[i].position.y = GetRandomValue(15, MAX_OFFSET);
        rainDrops[i].position.z = GetRandomValue(minZ, maxZ);
    }
}
 
// Update rain drops position and wrap around the camera
void UpdateRain(Vector3 cameraPosition, int activeRain) {
    float maxX, minX, maxZ, minZ;
    
    CalculateMinMax(&cameraPosition, &minX, &maxX, &minZ, &maxZ);

    for (int i = 0; i < activeRain; i++) {
        WrapRainPosition(&rainDrops[i].position.x, minX, maxX);
        WrapRainPosition(&rainDrops[i].position.z, minZ, maxZ);

        rainDrops[i].position.y -= 0.3f; // Rain speed

        // Reset rain drop position if it reaches the ground
        if (rainDrops[i].position.y < 0) {
            rainDrops[i].position.y = GetRandomValue(20, MAX_OFFSET);
        }
    }
}

void DrawRain(Model rain, Vector3 cameraPosition) {
    // Update rain drops 
    UpdateRain(cameraPosition, activeRain);

    for (int i = 0; i < activeRain; i++) {
        DrawModelEx(rain, rainDrops[i].position, (Vector3){1.0f, 0.0f, 0.0f}, 90.0f, RAINDROP_SCALE, DARKGRAY);
    }

    // Increase rain drops every frame till it reaches RAINDROP_COUNT
    if (activeRain < 200) {
        activeRain += 2;
    } else if (activeRain < RAINDROP_COUNT) {
        activeRain += 5;
    }
}