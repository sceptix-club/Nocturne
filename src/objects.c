#include "world/objects.h"

#define MAP_SIZE 25 // All four hidden objects will be within a circle of this radius
#define MIN_SPAWN_RADIUS 25
#define DISTANCE_THRESHOLD 10.0f
#define OBJECT_COUNT 4
#define OBJECT_Y 2.0f
#define DISAPPEAR_TIME 3.0f

typedef struct {
    Vector3 position;
    float rotation;
    bool isVisible;
    bool isFound;
    float foundTime;
} Object;

Object objects[OBJECT_COUNT];
bool allObjectsFound = false;

Model ObjectModel(Shader lightShader) {
    // Model object = LoadModel("assets/models/sphere.obj");

    // Material material = LoadMaterialDefault();
    // material.maps[MATERIAL_MAP_DIFFUSE].color = (Color){ 255, 0, 0, 255 };
    // object.materials[0] = material;
 
    // 3D cube for now
    Mesh cube = GenMeshCube(3.0f, 3.0f, 3.0f);
    Model object = LoadModelFromMesh(cube);
    object.materials[0].shader = lightShader;

    return object;
}

// Initialize hidden objects at random positions in 4 quadrants
void InitObjects() {
    float mapQuadrant = MAP_SIZE / 2.0f;
    float minQuadrant = MIN_SPAWN_RADIUS / 2.0f;

    for (int i = 0; i < OBJECT_COUNT; i++) {
        float offsetX = ((i & 2) == 0) ? -mapQuadrant : mapQuadrant;
        float offsetZ = ((i & 1) == 0) ? -mapQuadrant : mapQuadrant;

        float x = offsetX + (float)GetRandomValue(minQuadrant, mapQuadrant - minQuadrant);
        float z = offsetZ + (float)GetRandomValue(minQuadrant, mapQuadrant - minQuadrant);

        objects[i].position = (Vector3){ x, OBJECT_Y, z };
        objects[i].rotation = 0.0f;
        objects[i].isVisible = true;
        objects[i].isFound = false;
        objects[i].foundTime = 0.0f;
    }
}

void ObjectFound(Object *objects) {
    if(objects->isFound) {
        if (objects->foundTime == 0.0f) {
            objects->foundTime = GetTime();
        }

        float elapsedTime = GetTime() - objects->foundTime;
        if (elapsedTime > DISAPPEAR_TIME) {
            objects->isVisible = false;
        }
    }
}

// Update hidden objects and check if they are within the distance threshold
void UpdateObjects(Camera *camera) {
    allObjectsFound = true;

    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (objects[i].isVisible) {
            float distance = Vector3Distance(camera->position, objects[i].position);

            if (distance < DISTANCE_THRESHOLD) {
                objects[i].isFound = true;
            }

            if (objects[i].isVisible) {
                allObjectsFound = false;
            }

            if (objects[i].isFound) {
                ObjectFound(&objects[i]);
            }
        }
    }
}

void DrawObjects(Model object, Camera *camera) {
    UpdateObjects(camera);

    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (objects[i].isVisible) {
            DrawModelEx(object,
                        objects[i].position,
                        (Vector3){ 0.0f, 1.0f, 0.0f },
                        objects[i].rotation,
                        Vector3One(),
                        WHITE);
        }
    }
}

