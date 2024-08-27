#ifndef FIREFLY_H
#define FIREFLY_H

#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <raymath.h>

Model Firefly();
void InitFireflies(Vector3 cameraPosition);
void DrawFireflies(Model firefly, Vector3 cameraPosition);

#endif // FIREFLY_H