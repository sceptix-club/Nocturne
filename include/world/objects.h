#ifndef OBJECTS_H
#define OBJECTS_H

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

extern bool allObjectsFound;

Model ObjectModel(Shader lightShader);
Model MarkerModel();
void InitObjects();
void DrawObjects(Model object, Camera *camera);
void DrawMarkers(Model marker);

#endif