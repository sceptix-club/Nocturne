#ifndef OBJECTS_H
#define OBJECTS_H

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define OBJECT_COUNT 4

typedef struct {
    Model bone;
    Model ball;
    Model sign;
    Model grave;
} AllObjects;

typedef struct {
    float posX;
    float posZ;
} ExcludePos;

extern ExcludePos excludePos[OBJECT_COUNT];
extern bool allObjectsFound;
extern int *seq;

AllObjects ObjectModel(Shader lightShader);
Model MarkerModel();
void InitObjects();
void DrawObjects(AllObjects object, Camera *camera);
void DrawMarkers(Model marker);

#endif