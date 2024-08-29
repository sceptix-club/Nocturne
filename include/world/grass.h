#ifndef GRASS_H
#define GRASS_H

#include <raylib.h>

float Noise(float x, float y);
Model GrassBlade(Shader lightShader);
void InitGrass(Vector3 playerPos);
void UpdateGrassPatches(Vector3 playerPos, Vector3 modelPos);
void DrawGrassNew(Model grass);

#endif