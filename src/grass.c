#include "world/grass.h"

#define GRASSBLADE_COUNT 8000
#define PATCH_SIZE 50.0f
#define ANIM_SCALE 8.0f

typedef struct {
    Vector3 position;
    float scale;
    float rotation;
    float animationOffsetSin;
    float animationOffsetCos;
} GrassBlade;

GrassBlade grassBlades[GRASSBLADE_COUNT];
double time_init = 0.0f;

static inline float Noise(float x, float y) {
    return sinf(x * 0.1f) * cosf(y * 0.1f) * 5.0f;
}

Model GrassBladeModel(Shader lightShader) {
    Model grass = LoadModel("assets/grass.obj");
    grass.materials[0].shader = lightShader;
    return grass;
}

void InitGrass(Vector3 cameraPosition) {
    srand(time(NULL)); // Seed for random number generation
    // Initialize grass blades with random positions and scales
    for (int i = 0; i < GRASSBLADE_COUNT; i++) {
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float distance = GetRandomValue(0, (int)(PATCH_SIZE * 10)) / 10.0f;

        float x = cameraPosition.x + distance * cosf(angle);
        float z = cameraPosition.z + distance * sinf(angle);
        float noise = Noise(x, z);

        grassBlades[i].position.x = x + noise;
        grassBlades[i].position.z = z + noise;
        grassBlades[i].position.y = 0.0f;                       // Initial height
        grassBlades[i].scale = GetRandomValue(5,15); // Random scale 1.0 between  and 15.0
        grassBlades[i].rotation = GetRandomValue(10, 270);
        // Calculated here so that DrawGrassNew() doesn't have to call the
        // trigonometric functions NUM_GRASS_BLADES times
        grassBlades[i].animationOffsetSin = sinf(GetRandomValue(0, 30)) * DEG2RAD; // SIN(ANIM)
        grassBlades[i].animationOffsetCos = cosf(GetRandomValue(0, 30)) * DEG2RAD; // COS(ANIM)
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

    // SIN(BEND + ANIM) = SIN(BEND) * COS(ANIM) + COS(BEND) + SIN(ANIM)
    float sinBendFactor = sinf(time_init * 2.0f) * DEG2RAD * ANIM_SCALE;
    float cosBendFactor = cosf(time_init * 2.0f) * DEG2RAD * ANIM_SCALE;

    for (int i = 0; i < GRASSBLADE_COUNT; i++) {
        const float bendFactor = ((sinBendFactor * grassBlades[i].animationOffsetCos)
                               + (cosBendFactor * grassBlades[i].animationOffsetSin))
                               * ANIM_SCALE;
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
