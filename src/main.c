#include <rlgl.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>

#include "shaders/lights.h"
#include "world/skybox.h"
#include "world/ground.h"
#include "world/grass.h"
#include "world/firefly.h"

#define RLIGHTS_IMPLEMENTATION

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif
int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "ARTEMIS");

    Camera camera = {0};
    camera.position = (Vector3){ 0.0f, 4.5f, 0.0f };
    camera.target = (Vector3){ 0.185f, 1.0f, -1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 85.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Model ground = Ground();
    Model skybox = SkyBox();

    // Light above camera
    Shader light = SetLights();

    // display cam position
    Model ground = Ground(light);

    //  grass model
    Model grass = GrassBlade(light);

    // firefly model
    Model firefly = Firefly();

    //Initialize grass
    InitGrass(camera.target);

    //Initialize Ground
    InitGround(camera.target);

    //Initialize Fireflies
    InitFireflies(camera.target);

    DisableCursor();
    SetTargetFPS(60);

    // --------------------------------------------------------------------------------------

    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_F))
        {
            ToggleFullscreen();
        }

        UpdateCameraPro(&camera,
        (Vector3){
            (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
            (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,    
            (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
            (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
            0.0f                                                // Move up-down
        },
        (Vector3){
            // Edit delta X for faster sideways movement
            GetMouseDelta().x*0.1f,                            // Rotation: yaw
            GetMouseDelta().y*0.08f,                            // Rotation: pitch
            0.0f                                                // Rotation: roll
        },
        0.0f);                              // Move to target (zoom)

        //Updating grass patch
        UpdateGrassPatches(camera.target);

        lightShaderUpdate(camera, light);
        //Updating Ground
        UpdateGroundPatches(camera.target);
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);

                BeginShaderMode(light);
                rlDisableBackfaceCulling();

                rlDisableDepthMask();
                   DrawModel(skybox, (Vector3){0,0,0},20.0f,BLACK);
                rlEnableDepthMask();

                //Draw Grass
                DrawGrassNew(grass);
                rlEnableBackfaceCulling();
                //Draw Ground with backface culling
                DrawGround(ground);

                //Draw Fireflies
                DrawFireflies(firefly, camera.target);

            EndMode3D();


        EndDrawing();
    }
    UnloadModel(grass);

    UnloadModel(ground);
    UnloadModel(skybox);
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);

    CloseWindow();

    return 0;
}