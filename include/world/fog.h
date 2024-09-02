#ifndef FOG_H
#define FOG_H

#include "raylib.h"

Shader FogShader();
void fogShaderUpdate(Camera camera, Shader shader);

#endif // FOG_H