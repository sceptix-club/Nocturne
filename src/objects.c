#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include "world/objects.h"

#define GAMEPLAY_MAP_SIZE 500 // All four hidden objects will be within a circle of this radius
#define MIN_SPAWN_DISTANCE 100
#define NUM_OBJECTS 4

typedef struct {
    Vector3 position;
} object_t;

object_t objects[NUM_OBJECTS];

Model ObjectModel(Shader light) {
    Model object = LoadModel("assets/models/sphere.obj");

    // Create a default material
    Material material = LoadMaterialDefault();

    // Set the diffuse colour to a bright, glowing colour
    material.maps[MATERIAL_MAP_DIFFUSE].color = (Color){ 255, 0, 0, 255 };

    // Set lightshader
    // material.shader = light;

    object.materials[0] = material;

    return object;
}

void InitObjects() {
    srand(time(NULL));

    for (int i = 0; i < NUM_OBJECTS; i++) {
        // Generate random polar cooridnates for each object
        float angle = GetRandomValue(0, 359) * DEG2RAD;
        float distance = GetRandomValue(MIN_SPAWN_DISTANCE, GAMEPLAY_MAP_SIZE);
        float x = cosf(angle) * distance;
        float z = sinf(angle) * distance;
        objects[i].position.x = x;
        // This it the height of the object's origin.
        objects[i].position.y = 10;
        objects[i].position.z = z;
    }
}

void DrawObjects(Model object) {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        DrawModel(
            object,
            objects[i].position,
            10.0f,
            WHITE
        );
    }
}