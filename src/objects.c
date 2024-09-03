#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "world/objects.h"

#define GAMEPLAY_MAP_SIZE 500 // All four hidden objects will be within a circle of this radius
#define MIN_SPAWN_DISTANCE 100
#define DISTANCE_THRESHOLD 10.0f
#define NUM_OBJECTS 4

typedef struct {
    Vector3 position;
    bool visible;
} object_t;

object_t objects[NUM_OBJECTS];

Model ObjectModel() {
    Model object = LoadModel("assets/models/sphere.obj");

    // Create a default material
    Material material = LoadMaterialDefault();
    // Set the diffuse colour to a bright, glowing colour
    material.maps[MATERIAL_MAP_DIFFUSE].color = (Color){ 255, 0, 0, 255 };

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
        objects[i].visible = false;
    }
}

bool DrawObjects(Model object, Camera *camera) {
    Vector2 cameraPosition = (Vector2){ camera->position.x, camera->position.z };
    float distance = 11.0f;

    for (int i = 0; i < NUM_OBJECTS; i++) {
        distance = Vector2Distance(
            cameraPosition,
            (Vector2) { objects[i].position.x, objects[i].position.z }
        );

        DrawModel(
            object,
            objects[i].position,
            10.0f,
            WHITE
        );
    }

    return distance <= DISTANCE_THRESHOLD;
}