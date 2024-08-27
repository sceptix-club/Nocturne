#include "../include/world/firefly.h"

#define FIREFLY_COUNT 50
#define FIREFLY_RADIUS 0.07f
#define MAX_OFFSET 30.0f
#define MAX_HEIGHT 15.0f

typedef struct Firefly {
    Vector3 position;
} firefly;

firefly fireflies[FIREFLY_COUNT];

Model Firefly() {
    Mesh sphere = GenMeshSphere(FIREFLY_RADIUS, 8, 8);
    Model firefly = LoadModelFromMesh(sphere);
    
    return firefly;
}

void InitFireflies(Vector3 cameraPosition) {
    srand(time(NULL));

    for (int i = 0; i < FIREFLY_COUNT; i++) { // Initialize fireflies with random positions around the camera
        float fireflyX = cameraPosition.x + (float)(rand() % (int)(MAX_OFFSET * 2)) - MAX_OFFSET;
        float fireflyY = 0 + (float)(rand() % (int)(MAX_HEIGHT - 0 + 1));
        float fireflyZ = cameraPosition.z + (float)(rand() % (int)(MAX_OFFSET * 2)) - MAX_OFFSET;

        fireflies[i].position = (Vector3){fireflyX, fireflyY, fireflyZ}; 
    }
}

void UpdateFireflies(Vector3 cameraPosition) {
    for (int i = 0; i < FIREFLY_COUNT; i++) {
        if (fireflies[i].position.x > cameraPosition.x + MAX_OFFSET) {
            fireflies[i].position.x = cameraPosition.x - MAX_OFFSET;
        } else if (fireflies[i].position.x < cameraPosition.x - MAX_OFFSET) {
            fireflies[i].position.x = cameraPosition.x + MAX_OFFSET;
        }

        if (fireflies[i].position.z > cameraPosition.z + MAX_OFFSET) {
            fireflies[i].position.z = cameraPosition.z - MAX_OFFSET;
        } else if (fireflies[i].position.z < cameraPosition.z - MAX_OFFSET) {
            fireflies[i].position.z = cameraPosition.z + MAX_OFFSET;
        }
    }
}

void DrawFireflies(Model firefly, Vector3 cameraPosition) {
    UpdateFireflies(cameraPosition);

    for (int i = 0; i < FIREFLY_COUNT; i++) {
        DrawModel(firefly, fireflies[i].position, 1.0f, YELLOW);
    }
}
