#ifndef GROUND_H
#define GROUND_H

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define GROUND_SIZE 10.0f
#define CAMERA_EDGE_OFFSET 5.0f

typedef struct ProceduralGround
{
    Model model;
    Vector3 position;
} ProceduralGround;

extern ProceduralGround alivePlanes[6];
extern int alivePlanesCount;

// function to add ground procedurally
Model Ground(Vector3 cameraPosition);

#endif // GROUND_H