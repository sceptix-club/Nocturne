#ifndef GRASS_H
#define GRASS_H

#include <raylib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raymath.h>

#define GRASSBLADE_COUNT 8000
#define PATCH_SIZE 50.0f
#define ANIM_SCALE 8.0f

typedef struct {
    Vector3 position;
    float scale;
    float rotation;
    float animationOffsetSin;
    float animationOffsetCos;
    bool active;
} GrassBlade;

// Declare the variables as extern
extern GrassBlade grassBlades[GRASSBLADE_COUNT];
extern double time_init;

Model GrassBladeModel(Shader lightShader);
void InitGrass(Vector3 cameraPosition);
void DrawGrass(Model grass, Vector3 cameraPosition);

#endif
