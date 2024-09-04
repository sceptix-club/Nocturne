#ifndef RAIN_H
#define RAIN_H

#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

extern int activeRainDrops;

Model RainModel(Shader lightShader);
void ResetActiveRainDrops();
void InitRain(Vector3 cameraPosition);
void DrawRain(Model rain, Vector3 cameraPosition);

#endif // RAIN_H