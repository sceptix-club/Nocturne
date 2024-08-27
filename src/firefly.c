#include "../include/world/firefly.h"

#define FIREFLY_COUNT 50
#define FIREFLY_RADIUS 0.07f // Size of the firefly
#define MAX_OFFSET 30.0f // Maximum distance from the camera
#define MAX_HEIGHT 15.0f // Maximum height of the firefly
#define STEP_SIZE 0.002f // Step size for Brownian motion (smaller value = smoother motion)
#define BROWNIAN_VARIANCE 0.003f // Variance for Brownian motion (smaller value = slower speed)
#define BOUNCE_DAMPING 0.03f // Damping factor for bouncing off the ground and ceiling

typedef struct Firefly {
    Vector3 position;
    Vector3 velocity;
} firefly;

firefly fireflies[FIREFLY_COUNT];

Model Firefly() {
    Mesh sphere = GenMeshSphere(FIREFLY_RADIUS, 8, 8);
    Model firefly = LoadModelFromMesh(sphere);
    
    return firefly;
}

// Helper function to apply Brownian motion
static inline void BrownianMotionFirefly(firefly *f) {
    f->position = Vector3Add(f->position, f->velocity);
    f->velocity =Vector3AddValue(f->velocity, ((float)rand() / RAND_MAX - 0.5f) * BROWNIAN_VARIANCE); // small brownian motion added to velocity
}

// Helper function to wrap firefly position around the camera
static inline void WrapFireflyPosition(float *position, float min, float max) {
    if (*position > max) {
        *position = min;
    } else if (*position < min) {
        *position = max;
    }
}

// Initialize fireflies with random positions and velocities
void InitFireflies(Vector3 cameraPosition) {
    srand(time(NULL));

    for (int i = 0; i < FIREFLY_COUNT; i++) { 
        float fireflyX = cameraPosition.x + (float)(rand() % (int)(MAX_OFFSET * 2)) - MAX_OFFSET;
        float fireflyY = 2 + (float)(rand() % (int)(MAX_HEIGHT - 2 + 1));
        float fireflyZ = cameraPosition.z + (float)(rand() % (int)(MAX_OFFSET * 2)) - MAX_OFFSET;

        fireflies[i].position = (Vector3){fireflyX, fireflyY, fireflyZ};
        fireflies[i].velocity = (Vector3){0.0f, 0.0f, 0.0f};
    }
}

// Update fireflies position
void UpdateFireflies(Vector3 cameraPosition) {
    float maxX = cameraPosition.x + MAX_OFFSET;
    float minX = cameraPosition.x - MAX_OFFSET;
    float maxZ = cameraPosition.z + MAX_OFFSET;
    float minZ = cameraPosition.z - MAX_OFFSET;

    for (int i = 0; i < FIREFLY_COUNT; i++) {
        // Apply Brownian motion
        BrownianMotionFirefly(&fireflies[i]);

        // Wrap fireflies position around the camera
        WrapFireflyPosition(&fireflies[i].position.x, minX, maxX);
        WrapFireflyPosition(&fireflies[i].position.z, minZ, maxZ);

        // Clamp firefly position to the ground and max height
        if (fireflies[i].position.y < 2) {
            fireflies[i].position.y = 2;
            fireflies[i].velocity.y *= -BOUNCE_DAMPING; // Bounce off the ground
        } 
        if (fireflies[i].position.y > MAX_HEIGHT) {
            fireflies[i].position.y = MAX_HEIGHT;
            fireflies[i].velocity.y *= -BOUNCE_DAMPING; // Bounce off the ceiling
        }
    }
}

// Draw fireflies around the camera
void DrawFireflies(Model fireflyModel, Vector3 cameraPosition) {
    UpdateFireflies(cameraPosition);

    for (int i = 0; i < FIREFLY_COUNT; i++) {
        DrawModel(fireflyModel, fireflies[i].position, 1.0f, YELLOW);
    }
}
