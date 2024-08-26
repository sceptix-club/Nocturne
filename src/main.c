#include <raylib.h>
#include <rcamera.h>

#include "rlgl.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#include "shaders/lights.h"
#include "world/skybox.h"
#include "world/ground.h"
#include "world/grass.h"
#include "stdio.h"

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "ARTEMIS");

    Camera camera = {0};
    camera.position = (Vector3){ -15.0f, 10.0f, 2.0f };
    camera.target = (Vector3){ 0.185f, 1.5f, -1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    int cameraMode = CAMERA_FIRST_PERSON;

    // Model ground = Ground();
    Model skybox = SkyBox();

    // Light above camera
    Shader light = SetLights();


    // display cam position
    char cameraPosition[100];

    //grass model
    Model grass = GrassBlade(light);
    //Initialize grass
    InitGrass(camera.target,light);

    DisableCursor();
    SetTargetFPS(60);

    // --------------------------------------------------------------------------------------

    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_F))
        {
            ToggleFullscreen();
        }

        UpdateCamera(&camera, cameraMode);
        //Updating grass patch
        UpdateGrassPatches(camera.target);
        lightShaderUpdate(camera, light);

        // Plane below the player
        Ground(camera.position);

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
                
                for (int i = 0; i < alivePlanesCount; i++) {
                    DrawModel(alivePlanes[i].model, alivePlanes[i].position, 1.0f, WHITE);
                }

            EndMode3D();
        DrawFPS(5,5);

        // Below prints camera position
        sprintf(cameraPosition, "cam-pos: (%.2f, %.2f, %.2f)", camera.position.x, camera.position.y, camera.position.z);
        DrawText(cameraPosition, 100, 5, 20, WHITE );

        EndDrawing();
    }
    UnloadModel(grass);

    // Unload Ground
    for (int i = 0; i < alivePlanesCount; i++) {
        UnloadModel(alivePlanes[i].model);
    }

    UnloadModel(skybox);
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);

    CloseWindow();

    return 0;
}