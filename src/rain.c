#include "world/rain.h"
#include "utils/wrapPosition.h"

#define RAINDROP_COUNT 1000
#define MAX_OFFSET 30.0f
#define START_RAIN_COUNT 0

typedef struct RainDrop {
    Vector3 position;
} RainDrop;

RainDrop rainDrops[RAINDROP_COUNT];
int activeRain = START_RAIN_COUNT;


Model RainModel() {
    Mesh plane = GenMeshPlane(0.03f, 0.3f, 1, 1);
    Model rain = LoadModelFromMesh(plane);

    return rain;
}

inline void ResetActiveRainDrops() {
    activeRain = START_RAIN_COUNT;
}

// Initialize rain drops positions
void InitRain(Vector3 cameraPosition) {
    float maxX, minX, maxZ, minZ;
    CalculateMinMax(&cameraPosition, &minX, &maxX, &minZ, &maxZ, MAX_OFFSET);

    for (int i = 0; i < RAINDROP_COUNT; i++) {
        rainDrops[i].position.x = GetRandomValue(minX, maxX);
        rainDrops[i].position.y = GetRandomValue(15, MAX_OFFSET);
        rainDrops[i].position.z = GetRandomValue(minZ, maxZ);
    }
}
 
// Update rain drops position and wrap around the camera
void UpdateRain(Vector3 cameraPosition, int activeRain) {
    float maxX, minX, maxZ, minZ;
    
    CalculateMinMax(&cameraPosition, &minX, &maxX, &minZ, &maxZ, MAX_OFFSET);

    for (int i = 0; i < activeRain; i++) {
        WrapPosition(&rainDrops[i].position.x, minX, maxX);
        WrapPosition(&rainDrops[i].position.z, minZ, maxZ);

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
        DrawModelEx(rain, 
        rainDrops[i].position, 
        (Vector3){1.0f, 0.0f, 0.0f}, 
        90.0f, 
        Vector3One(), 
        DARKGRAY);
    }

    // Increase rain drops every frame till it reaches RAINDROP_COUNT
    if (activeRain < 200) {
        activeRain += 2;
    } else if (activeRain < RAINDROP_COUNT) {
        activeRain += 5;
    }
}