#include "world/grass.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raymath.h>
#define NUM_GRASS_BLADES 8000
#define PATCH_SIZE 50.0f

typedef struct
{
    Vector3 position;
    float scale;
    float rotation;
} grass_blade;

grass_blade grassBlades[NUM_GRASS_BLADES];
double time_init = 0.0f;

float Noise(float x, float y)
{
    return sinf(x * 0.1f) * cosf(y * 0.1f) * 5.0f;

}
Model GrassBlade(Shader lightShader)
{
    Model grass = LoadModel("assets/grass.obj");
    grass.materials[0].shader = lightShader;
    return grass;
}
void InitGrass(Vector3 playerPos)
{
    srand(time(NULL)); // Seed for random number generation
    // Initialize grass blades with random positions and scales
    for (int i = 0; i < NUM_GRASS_BLADES; i++)
    {
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float distance = GetRandomValue(0, (int)(PATCH_SIZE * 10)) / 10.0f;

        float x = playerPos.x + distance * cosf(angle);
        float z = playerPos.z + distance * sinf(angle);
        float noise = Noise(x, z);

        grassBlades[i].position.x = x + noise;
        grassBlades[i].position.z = z + noise;
        grassBlades[i].position.y = 0.0f;                       // Initial height
        grassBlades[i].scale = GetRandomValue(5,15); // Random scale 1.0 between  and 15.0
        grassBlades[i].rotation = GetRandomValue(10, 270);
    }
}
void UpdateGrassPatches(Vector3 playerPos)
{
        for (int i = 0; i < NUM_GRASS_BLADES; i++)
    {
        if (grassBlades[i].position.x < playerPos.x - PATCH_SIZE / 2)
        {
            grassBlades[i].position.x += PATCH_SIZE;
        }
        else if (grassBlades[i].position.x > playerPos.x + PATCH_SIZE / 2)
        {
            grassBlades[i].position.x -= PATCH_SIZE;
        }

        if (grassBlades[i].position.z < playerPos.z - PATCH_SIZE / 2)
        {
            grassBlades[i].position.z += PATCH_SIZE;
        }
        else if (grassBlades[i].position.z > playerPos.z + PATCH_SIZE / 2)
        {
            grassBlades[i].position.z -= PATCH_SIZE;
        }
    }
}
void DrawGrassNew(Model grass)
{
    time_init += GetFrameTime();
    float bendFactor = sinf(time_init * 2.0f) * 3.0f * DEG2RAD;
    for (int i = 0; i < NUM_GRASS_BLADES; i++)
    {
        DrawModelEx(grass, grassBlades[i].position, (Vector3){1.0f, 0.0f, 0.0f}, (grassBlades[i].rotation * bendFactor), (Vector3){grassBlades[i].scale, grassBlades[i].scale, grassBlades[i].scale}, DARKGRAY);
    }
}
