#include "../include/world/ground.h"

ProceduralGround alivePlanes[6];
int alivePlanesCount = 0;
 
// this just creates a new ground plane and adds it to the alivePlanes array (active planes)
void AddGround(Vector3 groundPosition) {
    if (alivePlanesCount < 6) {
        Mesh floor = GenMeshPlane(GROUND_SIZE, GROUND_SIZE, 1, 1);
        Model ground = LoadModelFromMesh(floor);
        ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = DARKGREEN;

        alivePlanes[alivePlanesCount].model = ground;
        alivePlanes[alivePlanesCount].position.x = groundPosition.x - GROUND_SIZE / 2;
        alivePlanes[alivePlanesCount].position.y = groundPosition.y;
        alivePlanes[alivePlanesCount].position.z = groundPosition.z;
        alivePlanesCount++;
    printf("Added ground at position: (%.2f, %.2f, %.2f)\n", groundPosition.x, groundPosition.y, groundPosition.z);
    }
}

// this removes a ground plane from the alivePlanes array (active planes)
void RemoveGround(int groundIndex) {
    if (groundIndex < alivePlanesCount) {
        UnloadModel(alivePlanes[groundIndex].model);
        for (int i = groundIndex; i < alivePlanesCount - 1; i++) {
            alivePlanes[i] = alivePlanes[i + 1];
        }
        alivePlanesCount--;
    }
}

// this updates the ground planes based on the camera position
// TODO: this function is not working as expected, needs to be fixed
void UpdateGroundFromCameraPosition(Vector3 cameraPosition) {
    for (int groundIndex = 0; groundIndex < alivePlanesCount; groundIndex++) {
        Vector3 groundPosition = alivePlanes[groundIndex].position;
        if(Vector3Distance(cameraPosition, groundPosition) > CAMERA_EDGE_OFFSET) {
            RemoveGround(groundIndex);
            groundIndex--;
        }
    }

    // Check if ground exists at the new position
    // TODO : this shit needs to fixed, some better calculation
    Vector3 newGroundPosition = {cameraPosition.x, cameraPosition.y, cameraPosition.z};
    newGroundPosition.x = (int)(newGroundPosition.x / GROUND_SIZE) * GROUND_SIZE;
    newGroundPosition.z = (int)(newGroundPosition.z / GROUND_SIZE) * GROUND_SIZE;


    int groundExists = 0;
    for (int i = 0; i < alivePlanesCount; i++) {
        if(Vector3Distance(newGroundPosition, alivePlanes[i].position) < GROUND_SIZE/ 2) {
            groundExists = 1;
            break;
        }
    }

    if (!groundExists) {
        AddGround(newGroundPosition);
    }
}

// this
Model Ground(Vector3 cameraPosition){
    UpdateGroundFromCameraPosition(cameraPosition);

    if (alivePlanesCount > 0) {
        return alivePlanes[0].model;
    } else {
        AddGround((Vector3){ 0.0f, 0.0f, 0.0f });
        return alivePlanes[0].model;
    }
}

/*
Model Ground()
{
    Mesh floor = GenMeshPlane(32.0f, 32.0f, 1, 1);
    Model ground = LoadModelFromMesh(floor);

    // Texture2D grassfloor = LoadTexture("assets/ground.png");
    // ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassfloor;
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = DARKGREEN;

    return ground;
}
*/
