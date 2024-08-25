#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#if defined(PLATFORM_DESKTOP)    //Checks for the GLSL versuin
    #define GLSL_VERSION            330
#else                              // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

Model SkyBox(){
    // Load skybox model
    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model skybox = LoadModelFromMesh(cube);

    // static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format);

    skybox.materials[0].shader = LoadShader(TextFormat("resources/glsl%i/skybox.fs",GLSL_VERSION),TextFormat("resources/glsl%i/skybox.fs",GLSL_VERSION));
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader,"environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader,"doGamma"), (int[1]){0}, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader,"vflipped"), (int[1]){1}, SHADER_UNIFORM_INT);
    Shader shdrCubemap = LoadShader(TextFormat("resources/glsl%i/skybox.fs", GLSL_VERSION), TextFormat("resources/glsl%i/cubemap.fs", GLSL_VERSION));
    SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){ 0 }, SHADER_UNIFORM_INT);


    Image img = LoadImage("assets/sky.png");
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(img);

    return skybox;
}
