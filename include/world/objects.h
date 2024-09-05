#ifndef OBJECTS_H
#define OBJECTS_H

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>

typedef struct {
    Model bone;
    Model ball;
    Model sign;
    Model grave;
} AllObjects;

extern bool allObjectsFound;

AllObjects ObjectModel(Shader lightShader);
Model MarkerModel();
void InitObjects();
void DrawObjects(AllObjects object, Camera *camera);
void DrawMarkers(Model marker);

#endif