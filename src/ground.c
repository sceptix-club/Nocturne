#include "../include/world/ground.h"

ProceduralGround alivePlanes[9];
int alivePlanesCount = 0;
 
// this just creates a new ground plane and adds it to the alivePlanes array (active planes)
void AddGround(Vector3 groundPosition) {
    if (alivePlanesCount < 9) {
        Mesh floor = GenMeshPlane(GROUND_SIZE, GROUND_SIZE, 1, 1);
        Model ground = LoadModelFromMesh(floor);
        ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = DARKGREEN;

        alivePlanes[alivePlanesCount].model = ground;
        alivePlanes[alivePlanesCount].position = groundPosition;
        alivePlanesCount++;
    }
}

// this removes a ground plane from the alivePlanes array (active planes)
void RemoveGround(int groundIndex) {
    if (groundIndex >= 0 && groundIndex < alivePlanesCount) {
        UnloadModel(alivePlanes[groundIndex].model);

        // this shifting is needed, maybe implement a linked list in future :(
        for (int i = groundIndex; i < alivePlanesCount - 1; i++) { 
            alivePlanes[i] = alivePlanes[i + 1];
        }
        alivePlanesCount--; 
    }
}

// this updates the ground planes based on the camera position
void UpdateGroundFromCameraPosition(Vector3 cameraPosition) {
    /*
    this thing below calculates the ground position based on the camera position as a grid
    explanation:
    1. newGroundPosition is set to the camera position with y set to 0 (ground height)
    2. newGroundPosition.x is divided by GROUND_SIZE and then floored to get the grid position
    3. newGroundPosition.x is then multiplied by GROUND_SIZE to get the actual position of the grid 
    */
    Vector3 curCameraPosition = {cameraPosition.x, 0.0f, cameraPosition.z};
    curCameraPosition.x = (int)(curCameraPosition.x / GROUND_SIZE) * GROUND_SIZE;
    curCameraPosition.z = (int)(curCameraPosition.z / GROUND_SIZE) * GROUND_SIZE;

    // create a check for the 3x3 grid of ground planes
    Vector3 newGroundPositions[9];
    int newGroundCount = 0;

    // this loop creates a 3x3 grid of ground planes around the camera
    for (float x = curCameraPosition.x - GROUND_SIZE; x <= curCameraPosition.x + GROUND_SIZE; x += GROUND_SIZE) {
        for (float z = curCameraPosition.z - GROUND_SIZE; z <= curCameraPosition.z + GROUND_SIZE; z += GROUND_SIZE) {
            Vector3 newGroundPosition = {x, 0.0f, z};
            newGroundPositions[newGroundCount++] = newGroundPosition;
        }
    }

    // if the ground plane is not in the 3x3 grid, remove it
    for (int groundIndex = 0; groundIndex < alivePlanesCount; groundIndex++) {
        int groundExists = 0;
        for (int i = 0; i < newGroundCount; i++) {
            if (Vector3Distance(alivePlanes[groundIndex].position, newGroundPositions[i]) < GROUND_SIZE / 2) {
                groundExists = 1;
                break;
            }
        }
        if (!groundExists) {
            RemoveGround(groundIndex--);
        }
    }

    // finally, if the ground plane is not in the alivePlanes array, add it
    for (int i = 0; i < newGroundCount; i++) {
        int groundExists = 0;
        for (int groundIndex = 0; groundIndex < alivePlanesCount; groundIndex++) {
            if (Vector3Distance(newGroundPositions[i], alivePlanes[groundIndex].position) < GROUND_SIZE / 2) {
                groundExists = 1;
                break;
            }
        }
        if (!groundExists) {
            AddGround(newGroundPositions[i]);
        }
    }
}

// this function is called in the drawing loop in main.c
Model Ground(Vector3 cameraPosition, Vector3 *prevCameraPosition) {
    if (!Vector3Equals(cameraPosition, *prevCameraPosition)) {
        UpdateGroundFromCameraPosition(cameraPosition);
        *prevCameraPosition = cameraPosition;
    }

    if (alivePlanesCount > 0) {
        return alivePlanes[0].model;
    } else {
        AddGround((Vector3){ 0.0f, 0.0f, 0.0f });
        return alivePlanes[0].model;
    }
}
