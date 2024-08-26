#include "../include/world/skybox.h"

#if defined(PLATFORM_DESKTOP)    //Checks for the GLSL versuin
    #define GLSL_VERSION            330
#else                              // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

Model SkyBox(){
    // Load skybox model
    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model skybox = LoadModelFromMesh(cube);

    skybox.materials[0].shader = LoadShader(TextFormat("resources/shaders/glsl%i/skybox.vs",GLSL_VERSION),TextFormat("resources/shaders/glsl%i/skybox.fs",GLSL_VERSION));
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader,"environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader,"doGamma"), (int[1]){0}, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader,"vflipped"), (int[1]){1}, SHADER_UNIFORM_INT);

    Image img = LoadImage("assets/skybox_moon.png");
    TextureCubemap cubeMaptexture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(img);
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = cubeMaptexture;
    return skybox;
}