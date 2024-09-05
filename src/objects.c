#include "world/objects.h"
#include "utils/sequence.h"

#define MAP_SIZE 30
#define MIN_SPAWN_RADIUS 25
#define DISTANCE_THRESHOLD 6.0
#define OBJECT_COUNT 4
#define OBJECT_Y 0.0f
#define OBJECT_SCALE (Vector3){ 3.0f, 3.0f, 3.0f }
#define FOUND_TIME 2.0f

typedef struct {
    int id;
    Vector3 position;
    bool isFound;
    bool isNextToFind;
    float foundTime;
} Object;

typedef struct {
    bool isVisible;
} Marker;

Object objects[OBJECT_COUNT];
Marker markers[OBJECT_COUNT];
AllObjects objectModel;
bool allObjectsFound = false;

static float markerPositionY = 0.0f;
static float markerRotationY = 0.0f;
int *seq;

AllObjects ObjectModel(Shader lightShader) {
    const char *modelPaths[OBJECT_COUNT] = {
        "assets/models/bone.obj",
        "assets/models/ball.obj",
        "assets/models/sign.obj",
        "assets/models/grave.obj"
    };

    Model models[OBJECT_COUNT];
    for (int i = 0; i < OBJECT_COUNT; i++) {
        models[i] = LoadModel(modelPaths[i]);
        models[i].materials[0].shader = lightShader;
    }

    return (AllObjects) {
        .bone = models[0],
        .ball = models[1],
        .sign = models[2],
        .grave = models[3]
    };
}

Model MarkerModel() {
    Mesh body = GenMeshCube(0.3f, 0.3f, 0.3f);
    return LoadModelFromMesh(body);
}

void InitObjects() {
    float mapQuadrant = MAP_SIZE / 2.0f;
    float minQuadrant = MIN_SPAWN_RADIUS / 2.0f;

    seq = GenerateRandomSequence(OBJECT_COUNT, 0, OBJECT_COUNT - 1);
    if (!seq) {
        puts("Error: Sequence generation failed.");
        return;
    }

    for (int i = 0; i < OBJECT_COUNT; i++) {
        float offsetX = (i & 2) ? mapQuadrant : -mapQuadrant;
        float offsetZ = (i & 1) ? mapQuadrant : -mapQuadrant;

        float x = offsetX + GetRandomValue(minQuadrant, mapQuadrant - minQuadrant);
        float z = offsetZ + GetRandomValue(minQuadrant, mapQuadrant - minQuadrant);

        objects[i] = (Object){
            .id = i,
            .position = (Vector3){ x, OBJECT_Y, z },
            .isFound = false,
            .isNextToFind = (i == seq[0]),
            .foundTime = 0.0f
        };

        markers[i].isVisible = false;
    }
}

void UpdateMarker(Object *obj) {
    float time = GetTime();
    float rotationSpeed = 0.5f;
    float oscillationSpeed = 0.5f;
    markerPositionY = sinf(time * 0.5f) * oscillationSpeed + 6.0f;
    markerRotationY = fmodf(markerRotationY + rotationSpeed, 360.0f);

    if (time - obj->foundTime > FOUND_TIME) {
        markers[obj->id].isVisible = true;
    }
}

void UpdateObjects(Camera *camera) {
    allObjectsFound = true;

    for (int i = 0; i < OBJECT_COUNT; i++) {
        Object *obj = &objects[i];
        float distance = Vector3Distance(camera->position, obj->position);

        if (distance < DISTANCE_THRESHOLD && !obj->isFound) {
            obj->isFound = true;
            obj->foundTime = GetTime();

            if (i + 1 < OBJECT_COUNT) {
                objects[i + 1].isNextToFind = true;
            }

        }

        if (obj->isFound) {
            UpdateMarker(obj);
        } else {
            allObjectsFound = false;
        }
    }
}

void DrawObjects(AllObjects objectsModels, Camera *camera) {
    UpdateObjects(camera);

    Model objectModels[OBJECT_COUNT] = { objectsModels.bone, objectsModels.ball, objectsModels.sign, objectsModels.grave };

    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (objects[seq[i]].isNextToFind) {
            DrawModelEx(
                objectModels[i], 
                objects[seq[i]].position, 
                (Vector3){ 0.0f, 1.0f, 0.0f }, 
                0.0f, 
                OBJECT_SCALE, 
                WHITE
            );
        }
    }
}

void DrawMarkers(Model marker) {
    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (markers[i].isVisible) {
            DrawModelEx(
                marker, 
                (Vector3){ objects[i].position.x, markerPositionY, objects[i].position.z }, 
                (Vector3){ 0.0f, 1.0f, 0.0f }, 
                markerRotationY, 
                Vector3One(), 
                WHITE
            );
        }
    }
}
