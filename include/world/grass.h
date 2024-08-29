#ifndef GRASS_H
#define GRASS_H

#include <raylib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raymath.h>

Model GrassBladeModel(Shader lightShader);
void InitGrass(Vector3 cameraPosition);
void DrawGrass(Model grass, Vector3 cameraPosition);

#endif