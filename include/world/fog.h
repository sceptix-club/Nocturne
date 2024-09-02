#ifndef FOG_H
#define FOG_H

#include "raylib.h"

Shader FogShader();
void UpdateFogShader(Camera camera, Shader shader);

#endif // FOG_H