#ifndef LIGHTS_H
#define LIGHTS_H

#include <raylib.h>
#include <raymath.h>

Shader SetLights();
void UpdateLightShader(Camera camera, Shader lightShader);
Shader Cinematic(void);
Shader CutSceneShader(void);


#endif // LIGHTS_H