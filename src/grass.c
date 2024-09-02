#include "world/grass.h"

GrassBlade grassBlades[GRASSBLADE_COUNT];
double time_init = 0.0f;

static inline float Noise(float x, float y){
    return sinf(x * NOISE_SCALE) * cosf(y * NOISE_SCALE) * 5.0f;
}
 
// Check if the position is in a no grass zone
bool NoGrassZone(Vector3 position) {
    float noise = Noise(position.x, position.z);
    float normalizedNoise = (noise + 1.0f) / 2.0f;
    return normalizedNoise < 0.55f;
}

Model GrassBladeModel(Shader lightShader) {
    Model grass = LoadModel("assets/models/grass.obj");
    grass.materials[0].shader = lightShader;
    return grass;
}

// Initialize grass blades with random positions and scales
void InitGrass(Vector3 cameraPosition) {
    srand((unsigned int)time(NULL));
    
    int validGrassBlades = 0;

    while (validGrassBlades < GRASSBLADE_COUNT) {
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float distance = GetRandomValue(0, (int)(PATCH_SIZE * 10)) / 10.0f;

        float x = cameraPosition.x + distance * cosf(angle);
        float z = cameraPosition.z + distance * sinf(angle);
        float noise = Noise(x, z);

        Vector3 position = {x + noise, 0.0f, z + noise};

        if (!NoGrassZone(position)) {
            grassBlades[validGrassBlades].position = position;
            grassBlades[validGrassBlades].scale = GetRandomValue(5, 15); // Random scale between 5.0 and 15.0
            grassBlades[validGrassBlades].rotation = GetRandomValue(10, 270);
            grassBlades[validGrassBlades].animationOffsetSin = sinf(GetRandomValue(0, 30)) * DEG2RAD; // SIN(ANIM)
            grassBlades[validGrassBlades].animationOffsetCos = cosf(GetRandomValue(0, 30)) * DEG2RAD; // COS(ANIM)
            validGrassBlades++;
        }
    }
}

void UpdateGrassPatches(Vector3 cameraPosition) {
    for (int i = 0; i < GRASSBLADE_COUNT; i++){
        if (grassBlades[i].position.x < cameraPosition.x - PATCH_SIZE / 2) {
            grassBlades[i].position.x += PATCH_SIZE;
        } else if (grassBlades[i].position.x > cameraPosition.x + PATCH_SIZE / 2) {
            grassBlades[i].position.x -= PATCH_SIZE;
        }

        if (grassBlades[i].position.z < cameraPosition.z - PATCH_SIZE / 2){
            grassBlades[i].position.z += PATCH_SIZE;
        } else if (grassBlades[i].position.z > cameraPosition.z + PATCH_SIZE / 2) {
            grassBlades[i].position.z -= PATCH_SIZE;
        }
    }
}

void DrawGrass(Model grass, Vector3 cameraPosition) {
    UpdateGrassPatches(cameraPosition);

    time_init += GetFrameTime();

    float sinBendFactor = sinf(time_init * 2.0f) * DEG2RAD * ANIM_SCALE;
    float cosBendFactor = cosf(time_init * 2.0f) * DEG2RAD * ANIM_SCALE;

    for (int i = 0; i < GRASSBLADE_COUNT; i++) {
        const float bendFactor = ((sinBendFactor * grassBlades[i].animationOffsetCos)
                               + (cosBendFactor * grassBlades[i].animationOffsetSin))
                               * ANIM_SCALE;

        if(grassBlades[i].active)
        {
            DrawModelEx(
                grass,
                grassBlades[i].position,
                (Vector3){1.0f, 0.0f, 0.0f},
                (grassBlades[i].rotation * bendFactor),
                (Vector3){grassBlades[i].scale, grassBlades[i].scale, grassBlades[i].scale},
                DARKGRAY
            );
        }
    }
}
