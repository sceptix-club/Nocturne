#include "world/fog.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else
    #define GLSL_VERSION 100
#endif

Shader FogShader() {
    Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/fog.vs", GLSL_VERSION), TextFormat("resources/shaders/glsl%i/fog.fs", GLSL_VERSION));

    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "matModel");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocationAttrib(shader, "viewPos");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){0.2, 0.2, 0.2, 1.0}, SHADER_UNIFORM_VEC4);

    int fogColor = GetShaderLocation(shader, "fogColor");
    SetShaderValue(shader, fogColor, (float[4]){0.6, 0.2, 0.2, 1.0}, SHADER_UNIFORM_VEC4);

    int fogDensity = GetShaderLocation(shader, "fogDensity");
    SetShaderValue(shader, fogDensity, (float[1]){0.1}, SHADER_UNIFORM_FLOAT);


    return shader;
}   

void fogShaderUpdate(Camera camera, Shader shader) {
    float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
}

