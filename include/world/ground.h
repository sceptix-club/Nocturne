#ifndef GROUND_H
#define GROUND_H

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define GROUND_SIZE 10.0f  // nithin change ground size here
#define CAMERA_EDGE_OFFSET GROUND_SIZE / 2.0f

typedef struct ProceduralGround
{
    Model model;
    Vector3 position;
} ProceduralGround;

extern ProceduralGround alivePlanes[9];
extern int alivePlanesCount;

// function to add ground procedurally
Model Ground(Vector3 cameraPosition, Vector3 *prevCameraPosition);
void UpdateGroundFromCameraPosition(Vector3 cameraPosition);

#endif // GROUND_H