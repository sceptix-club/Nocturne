#ifndef GRASS_H
#define GRASS_H

#include <raylib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raymath.h>

typedef struct {
    Vector3 position;
    float scale;
    float rotation;
    float animationOffsetSin;
    float animationOffsetCos;
    bool active;
} GrassBlade;

// Declare the variables as extern
extern GrassBlade grass[];
extern double time_init;

Model GrassBladeModel(Shader lightShader);
void InitGrass(Vector3 cameraPosition);
void DrawGrass(Model grassBlade, Vector3 cameraPosition);

#endif
