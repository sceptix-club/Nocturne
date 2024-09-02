#include "world/fog.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else
    #define GLSL_VERSION 100
#endif

Shader FogShader() {
    Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/fog.vs", GLSL_VERSION), TextFormat("resources/shaders/glsl%i/fog.fs", GLSL_VERSION));

    shader.locs[SHADER_LOC_MATRIX_MODEL]  = GetShaderLocation(shader, "matModel");
    shader.locs[SHADER_LOC_VECTOR_VIEW]   = GetShaderLocation(shader, "viewPos");
    shader.locs[SHADER_LOC_MATRIX_VIEW]  = GetShaderLocation(shader, "viewMatrix");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){0.2f, 0.2f, 0.2f, 1.0f}, SHADER_UNIFORM_VEC4);

    int fogColorLoc = GetShaderLocation(shader, "fogColor");
    SetShaderValue(shader, fogColorLoc, (float[4]){1.0f, 0.0f, 0.0f, 1.0f}, SHADER_UNIFORM_VEC4);

    int fogDensityLoc = GetShaderLocation(shader, "fogDensity");
    SetShaderValue(shader, fogDensityLoc, (float[1]){1.0f}, SHADER_UNIFORM_FLOAT);

    int fogStartLoc = GetShaderLocation(shader, "fogStart");
    SetShaderValue(shader, fogStartLoc, (float[1]){4.0f}, SHADER_UNIFORM_FLOAT);

    int fogEndLoc = GetShaderLocation(shader, "fogEnd");
    SetShaderValue(shader, fogEndLoc, (float[1]){20.0f}, SHADER_UNIFORM_FLOAT);

    return shader;
}

void UpdateFogShader(Camera camera, Shader shader) {
    float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    Matrix viewMatrix = GetCameraMatrix(camera);
    SetShaderValueMatrix(shader, shader.locs[SHADER_LOC_MATRIX_VIEW], viewMatrix);
}