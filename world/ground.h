#ifndef GROUND_H
#define GROUND_H

#include <raylib.h>

// TODO: Perceptually infinite floor

Model Ground()
{
    Mesh floor = GenMestPlane(32.0f, 32.0f, 1, 1);
    Model ground = LoadModelFromMesd(floor);

    Texture2D grassfloor = LoadTexture("/assets/ground.png");
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassfloor;

    return ground;
}