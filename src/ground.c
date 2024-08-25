#include "../include/world/ground.h"

// TODO: Perceptually infinite floor

Model Ground()
{
    Mesh floor = GenMeshPlane(32.0f, 32.0f, 1, 1);
    Model ground = LoadModelFromMesh(floor);

    Texture2D grassfloor = LoadTexture("assets/ground.png");
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassfloor;

    return ground;
}
