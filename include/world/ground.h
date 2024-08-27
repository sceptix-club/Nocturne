#ifndef GROUND_H
#define GROUND_H
#include <raylib.h>

Model Ground(Shader lightShader);
void InitGround(Vector3 playerPos);
void UpdateGroundPatches(Vector3 playerPos);
void DrawGround(Model ground);

#endif