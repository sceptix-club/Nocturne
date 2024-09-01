#include "world/ground.h"

#define GROUND_COUNT 9
#define GROUND_SIZE 50.0f
#define BUFFER_SIZE 5.0f

typedef struct
{
    Vector3 position;
} Ground;

Ground grounds[GROUND_COUNT];

Model GroundModel(Shader lightShader) {
    Mesh plane = GenMeshPlane(GROUND_SIZE,GROUND_SIZE,2,2);
    Model ground = LoadModelFromMesh(plane);
    Texture2D groundTex = LoadTexture("assets/groundTex2.png");
    ground.materials[0].shader = lightShader;
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =  groundTex;
    

    return ground;
}

void InitGround(Vector3 cameraPosition) {
    grounds[0].position = cameraPosition;
    Vector3 offsets[8] = {
        (Vector3){ -GROUND_SIZE, 0.0f, -GROUND_SIZE }, // Top-left
        (Vector3){ 0.0f, 0.0f, -GROUND_SIZE },         // Top-center
        (Vector3){ GROUND_SIZE, 0.0f, -GROUND_SIZE },  // Top-right
        (Vector3){ -GROUND_SIZE, 0.0f, 0.0f },         // Middle-left
        (Vector3){ GROUND_SIZE, 0.0f, 0.0f },          // Middle-right
        (Vector3){ -GROUND_SIZE, 0.0f, GROUND_SIZE },  // Bottom-left
        (Vector3){ 0.0f, 0.0f, GROUND_SIZE },          // Bottom-center
        (Vector3){ GROUND_SIZE, 0.0f, GROUND_SIZE }    // Bottom-right
    };

    for(int i=1; i<=GROUND_COUNT; i++) {
        grounds[i].position = Vector3Add(cameraPosition, offsets[i-1]) ;
    }
}

void UpdateGroundPatches(Vector3 cameraPosition) {
    // Calculate the center cell position with the buffer
    int centerX = (int)((cameraPosition.x - BUFFER_SIZE) / GROUND_SIZE);
    int centerZ = (int)((cameraPosition.z - BUFFER_SIZE) / GROUND_SIZE);

    // Update ground patch positions based on the player position
    for (int i = 0; i < GROUND_COUNT; i++) {
        int offsetX = (i % 3) - 1;  // Offset in X direction for a 3x3 grid
        int offsetZ = (i / 3) - 1;  // Offset in Z direction for a 3x3 grid

        grounds[i].position = (Vector3){
            (centerX + offsetX) * GROUND_SIZE,
            0.0f,
            (centerZ + offsetZ) * GROUND_SIZE
        };
    }
}

void DrawGround(Model ground, Vector3 cameraPosition) {
    UpdateGroundPatches(cameraPosition);

    for(int i=0; i<GROUND_COUNT; i++) {
        DrawModel(ground,grounds[i].position, 1.0f,DARKGREEN);
    }
}