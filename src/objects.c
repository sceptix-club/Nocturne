#include "world/objects.h"

/*
    * Small map size for now
    * Increase MAP_SIZE for more object spread
    * Keep MIN_SPAWN_RADIUS less than MAP_SIZE
*/
#define MAP_SIZE 25
#define MIN_SPAWN_RADIUS 20
#define DISTANCE_THRESHOLD 8.0
#define OBJECT_COUNT 4
#define OBJECT_Y 0.0f
#define OBJECT_SCALE (Vector3){ 3.0f, 3.0f, 3.0f }
#define FOUND_TIME 2.0f

// Struct for hidden objects
typedef struct {
    int id;
    Vector3 position;
    bool isFound;
    float foundTime;
} Object;

// Struct for markers
typedef struct {
    bool isVisible;
} Marker;

Object objects[OBJECT_COUNT];
Marker markers[OBJECT_COUNT];
AllObjects objectModel;
bool allObjectsFound = false;

static float markerPositionY = 0.0f;
static float markerRotationY = 0.0f;




AllObjects ObjectModel(Shader lightShader) {
    // Bone model
    Model bone = LoadModel("assets/models/bone.obj");
    bone.materials[0].shader = lightShader;

    // Ball model
    Model ball = LoadModel("assets/models/ball.obj");
    ball.materials[0].shader = lightShader;

    // Sign Board model
    Model sign = LoadModel("assets/models/ball.obj");
    sign.materials[0].shader = lightShader;

    // Grave model
    Model grave = LoadModel("assets/models/grave.obj");
    grave.materials[0].shader = lightShader;

    objectModel = (AllObjects){
        .bone = bone,
        .ball = ball,
        .sign = sign,
        .grave = grave
    };

    return objectModel;
}

Model MarkerModel() {
    Mesh body = GenMeshCube(0.3f, 0.3f, 0.3f);
    Model marker = LoadModelFromMesh(body);

    return marker;
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
            .id = i,
            .position = (Vector3){ x, OBJECT_Y, z },
            .isFound = false,
            .foundTime = 0.0f
        };

        markers[i].isVisible = false;
    }
}

/*
    * Update the object's marker when found
    * Rotate and oscillate the marker
*/
void ObjectFound(Object *obj) {
    float time = GetTime();
    float rotationSpeed = 0.5f;
    float height = 6.0f;
    float oscillationSpeed = 0.5f;

    markerPositionY = sinf(time * 0.5f) * oscillationSpeed + height;
    markerRotationY = fmodf(markerRotationY + rotationSpeed, 360.0f);

    if(GetTime() - obj->foundTime > FOUND_TIME){
        markers[obj->id].isVisible = true;
    }
}

// Update hidden objects and check if they are within the distance threshold
void UpdateObjects(Camera *camera) {
    allObjectsFound = true;

    for (int i = 0; i < OBJECT_COUNT; i++) {
        Object *obj = &objects[i];

        float distance = Vector3Distance(camera->position, obj->position);
        if (distance < DISTANCE_THRESHOLD && !obj->isFound) {
            obj->isFound = true;
            obj->foundTime = (float)GetTime();
        }

        if (obj->isFound){
            ObjectFound(obj);
        } else {
            allObjectsFound = false;
        }
    }
}

void DrawObjects(AllObjects object, Camera *camera) {
    UpdateObjects(camera);

    DrawModelEx(object.bone, (Vector3){ objects[0].position.x, OBJECT_Y, objects[0].position.z }, (Vector3){ 0.0f, 1.0f, 0.0f }, 90.0f, OBJECT_SCALE, WHITE);
    DrawModelEx(object.ball, (Vector3){ objects[1].position.x, OBJECT_Y, objects[1].position.z }, (Vector3){ 0.0f, 1.0f, 0.0f }, 0.0f, OBJECT_SCALE, WHITE);
    DrawModelEx(object.sign, (Vector3){ objects[2].position.x, OBJECT_Y, objects[2].position.z }, (Vector3){ 0.0f, 1.0f, 0.0f }, 0.0f, OBJECT_SCALE, WHITE);
    DrawModelEx(object.grave, (Vector3){ objects[3].position.x, OBJECT_Y, objects[3].position.z }, (Vector3){ 0.0f, 1.0f, 0.0f }, 0.0f, OBJECT_SCALE, WHITE);
}

void DrawMarkers(Model marker) {
    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (markers[i].isVisible) {
            DrawModelEx(marker,
                        (Vector3){ objects[i].position.x,markerPositionY, objects[i].position.z },
                        (Vector3){ 0.0f, 1.0f, 0.0f },
                        markerRotationY,
                        Vector3One(),
                        WHITE);
        }
    }
}