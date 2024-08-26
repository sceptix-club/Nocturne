#include "../include/world/firefly.h"

#define FIREFLY_COUNT 100
#define MAX_RANGE 50.0f

typedef struct Firefly {
    Vector3 position;
} Firefly;

Firefly fireflies[FIREFLY_COUNT];

void InitFireflies(Vector3 cameraPosition) {
    srand(time(NULL));

    for (int i = 0; i < FIREFLY_COUNT; i++) {
        float fireflyX = cameraPosition.x + (float)(rand() % (int)(MAX_RANGE * 2)) - MAX_RANGE;
        float fireflyY = 0 + (float)(rand() % (int)(MAX_RANGE - 0 + 1));
        float fireflyZ = cameraPosition.z + (float)(rand() % (int)(MAX_RANGE * 2)) - MAX_RANGE;

        fireflies[i].position = (Vector3){fireflyX, fireflyY, fireflyZ};
    }
}

void RandomFirefliesMovement(){
    for (int i = 0; i < FIREFLY_COUNT; i++) {
        float newFireflyX = fireflies[i].position.x + (float)(rand() % 3) - 1;
        float newFireflyY = fireflies[i].position.y + (float)(rand() % 3) - 1;
        float newFireflyZ = fireflies[i].position.z + (float)(rand() % 3) - 1;

        fireflies[i].position = (Vector3){newFireflyX, newFireflyY, newFireflyZ};
    }
}

