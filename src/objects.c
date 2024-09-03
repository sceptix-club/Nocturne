#include "world/objects.h"

#define MAP_SIZE 25 // Small map size for now
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
        float offsetX = (i & 2) ? mapQuadrant : -mapQuadrant;
        float offsetZ = (i & 1) ? mapQuadrant : -mapQuadrant;

        float x = offsetX + (float)GetRandomValue(minQuadrant, mapQuadrant - minQuadrant);
        float z = offsetZ + (float)GetRandomValue(minQuadrant, mapQuadrant - minQuadrant);

        objects[i] = (Object){
            .position = (Vector3){ x, OBJECT_Y, z },
            .rotation = 0.0f,
            .isVisible = true,
            .isFound = false,
            .foundTime = 0.0f
        };
    }
}

void ObjectFound(Object *obj) {
    if (!obj->isFound) return;

    if (obj->foundTime == 0.0f) {
        obj->foundTime = GetTime();
    } else {
        float elapsedTime = GetTime() - obj->foundTime;
        if (elapsedTime > DISAPPEAR_TIME) {
            obj->isVisible = false;
        }
    }
}

// Update hidden objects and check if they are within the distance threshold
void UpdateObjects(Camera *camera) {
    allObjectsFound = true;
    float currentTime = GetTime();

    for (int i = 0; i < OBJECT_COUNT; i++) {
        Object *obj = &objects[i];

        if (obj->isVisible) {
            float distance = Vector3Distance(camera->position, obj->position);

            if (distance < DISTANCE_THRESHOLD) {
                obj->isFound = true;
            }

            if (obj->isFound) {
                ObjectFound(obj);
            }

            if (obj->isVisible) {
                allObjectsFound = false;
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
