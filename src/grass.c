#include "world/grass.h"
#include "world/objects.h"

#define GRASSBLADE_COUNT 8000
#define PATCH_SIZE 50.0f
#define ANIM_SCALE 8.0f
#define NOISE_SCALE 0.3f

typedef struct {
    Vector3 position;
    float scale;
    float rotation;
    float animationOffsetSin;
    float animationOffsetCos;
    bool active;
} GrassBlade;

GrassBlade grass[GRASSBLADE_COUNT];
float noiseOffset = 0.0f;
float excludedRadius[OBJECT_COUNT] = { 8.0f, 5.0f, 5.0f, 8.0f };




// --------------------------------------(Helper Functions)--------------------------------------

static inline float Noise(float x, float y){
    return sinf((x + noiseOffset) * NOISE_SCALE) * cosf((y + noiseOffset) * NOISE_SCALE) * 5.0f;
}
 
bool NoGrassZone(Vector3 position) {
    float noise = Noise(position.x, position.z);
    return noise > 1.0f;
}

bool IsObjectZone(float x, float z) {
    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (Vector2Distance((Vector2){x, z}, (Vector2){excludePos[i].posX, excludePos[i].posZ}) < excludedRadius[i]) {
            return true;
        }
    }

    return false;
}

// --------------------------------------(Grass Functions)--------------------------------------

Model GrassBladeModel(Shader lightShader) {
    Model grass = LoadModel("assets/models/grass.obj");
    grass.materials[0].shader = lightShader;
    return grass;
}

// Initialize grass blades with random positions and scales
void InitGrass(Vector3 cameraPosition) {
    srand(time(NULL));
    noiseOffset = (float)GetRandomValue(0, 1000) / 100.0f;

    float angle, distance, x, z, noise;
    Vector3 position;

    for (int i = 0; i < GRASSBLADE_COUNT; i++) {
        angle = GetRandomValue(0, 360) * DEG2RAD;
        distance = GetRandomValue(0, (int)(PATCH_SIZE * 10)) / 10.0f;

        x = cameraPosition.x + distance * cosf(angle);
        z = cameraPosition.z + distance * sinf(angle);
        noise = Noise(x, z);

        position = (Vector3){x + noise, 0.0f, z + noise};

        grass[i].position = position;
        grass[i].scale = GetRandomValue(5, 14);
        grass[i].rotation = GetRandomValue(0, 90);
        grass[i].animationOffsetSin = sinf(GetRandomValue(0, 30)) * DEG2RAD;
        grass[i].animationOffsetCos = cosf(GetRandomValue(0, 30)) * DEG2RAD;;
        grass[i].active = !(NoGrassZone(position) || IsObjectZone(position.x, position.z));

    }
}

// Update grass blades position and wrap around the camera
void UpdateGrass(Vector3 cameraPosition) {
    for (int i = 0; i < GRASSBLADE_COUNT; i++){
        if (grass[i].position.x < cameraPosition.x - PATCH_SIZE / 2) {
            grass[i].position.x += PATCH_SIZE;
        } else if (grass[i].position.x > cameraPosition.x + PATCH_SIZE / 2) {
            grass[i].position.x -= PATCH_SIZE;
        }

        if (grass[i].position.z < cameraPosition.z - PATCH_SIZE / 2){
            grass[i].position.z += PATCH_SIZE;
        } else if (grass[i].position.z > cameraPosition.z + PATCH_SIZE / 2) {
            grass[i].position.z -= PATCH_SIZE;
        }

        grass[i].active = !IsObjectZone(grass[i].position.x, grass[i].position.z);
    }
}

// Draw grass blades
void DrawGrass(Model grassBlade, Vector3 cameraPosition) {
    UpdateGrass(cameraPosition);

    // Calculate the bend factor for the grass animation
    float time = GetTime();
    float sinBendFactor = sinf(time * 2.0f) * DEG2RAD * ANIM_SCALE;
    float cosBendFactor = cosf(time * 2.0f) * DEG2RAD * ANIM_SCALE;

    for (int i = 0; i < GRASSBLADE_COUNT; i++) {
        if(grass[i].active) {
            // Grass animation
            const float bendFactor = (
                (sinBendFactor * grass[i].animationOffsetCos) +
                (cosBendFactor * grass[i].animationOffsetSin)
            ) * ANIM_SCALE; 

            DrawModelEx(
                grassBlade,
                grass[i].position,
                (Vector3){1.0f, 0.0f, 1.0f},
                (grass[i].rotation * bendFactor),
                (Vector3){grass[i].scale, grass[i].scale, grass[i].scale},
                DARKGRAY
            );
        }
    }
}
