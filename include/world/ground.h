#ifndef GROUND_H
#define GROUND_H

#include <raylib.h>
#include <raymath.h>

Model GroundModel(Shader lightShader);
void InitGround(Vector3 cameraPosition);
void DrawGround(Model ground, Vector3 cameraPosition);

#endif