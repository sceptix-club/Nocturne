#ifndef RAIN_H
#define RAIN_H

#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

Model Rain();
void InitRain(Vector3 cameraPosition);
void DrawRain(Model rain, Vector3 cameraPosition, int rainCount);

#endif // RAIN_H