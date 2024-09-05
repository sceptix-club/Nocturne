#include "world/grass.h"

#define GRASSBLADE_COUNT 8000
#define PATCH_SIZE 50.0f
#define ANIM_SCALE 8.0f
#define NOISE_SCALE 0.3f

GrassBlade grass[GRASSBLADE_COUNT];
double time_init = 0.0f;
float noiseOffset = 0.0f;

static inline float Noise(float x, float y){
    return sinf((x + noiseOffset) * NOISE_SCALE) * cosf((y + noiseOffset) * NOISE_SCALE) * 5.0f;
}
 
// Check if the position is in a no grass zone
bool NoGrassZone(Vector3 position) {
    float noise = Noise(position.x, position.z);
    return noise > 1.0f;
}

Model GrassBladeModel(Shader lightShader) {
    Model grass = LoadModel("assets/models/grass.obj");
    grass.materials[0].shader = lightShader;
    return grass;
}

// Initialize grass blades with random positions and scales
void InitGrass(Vector3 cameraPosition) {
    srand((unsigned int)time(NULL));

    noiseOffset = (float)GetRandomValue(0, 1000) / 100.0f;

    int validgrass = 0;

    while (validgrass < GRASSBLADE_COUNT) {
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float distance = GetRandomValue(0, (int)(PATCH_SIZE * 10)) / 10.0f;

        float x = cameraPosition.x + distance * cosf(angle);
        float z = cameraPosition.z + distance * sinf(angle);
        float noise = Noise(x, z);

        Vector3 position = {x + noise, 0.0f, z + noise};

        grass[validgrass].active = !NoGrassZone(position);
        grass[validgrass].position = position;
        grass[validgrass].scale = GetRandomValue(5, 15); // Random scale between 5.0 and 15.0
        grass[validgrass].rotation = GetRandomValue(0, 90);
        grass[validgrass].animationOffsetSin = sinf(GetRandomValue(0, 30)) * DEG2RAD; // SIN(ANIM)
        grass[validgrass].animationOffsetCos = cosf(GetRandomValue(0, 30)) * DEG2RAD; // COS(ANIM)
        validgrass++;

        grass[validgrass].active = true;
    }
}

void UpdateGrassPatches(Vector3 cameraPosition) {
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
    }
}

void DrawGrass(Model grassBlade, Vector3 cameraPosition) {
    UpdateGrassPatches(cameraPosition);

    time_init += GetFrameTime();

    float sinBendFactor = sinf(time_init * 2.0f) * DEG2RAD * ANIM_SCALE;
    float cosBendFactor = cosf(time_init * 2.0f) * DEG2RAD * ANIM_SCALE;

    for (int i = 0; i < GRASSBLADE_COUNT; i++) {
        const float bendFactor = ((sinBendFactor * grass[i].animationOffsetCos)
                               + (cosBendFactor * grass[i].animationOffsetSin))
                               * ANIM_SCALE;

        if(grass[i].active)
        {
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
