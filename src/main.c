#include <raylib.h>
#include <rcamera.h>
#include <stdio.h>

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
#include "world/firefly.h"
#include "world/rain.h"
#include "stdio.h"

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY} GameScreen;

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;
    GameScreen currentScreen = LOGO;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "ARTEMIS");
    int framesCounter = 0;

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
    Model firefly = FireflyModel();

    // rain model
    Model rain = RainModel();

    //Initialize grass
    InitGrass(camera.target);

    //Initialize Ground
    InitGround(camera.target);

    //Initialize Fireflies
    InitFireflies(camera.target);

    //Initialize Rain
    InitRain(camera.target);

    DisableCursor();
    SetTargetFPS(60);

    bool toggleRain = false;
    bool previousRain = false;

    // --------------------------------------------------------------------------------------

    while (!WindowShouldClose()) 
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!

                framesCounter++;    // Count frames

                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (framesCounter > 120)
                {
                    currentScreen = TITLE;
                }
            } break;
            case TITLE:
            {
                // TODO: Update TITLE screen variables here!

                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                // Press enter to change to ENDING screen
            } break;
            default: break;
        }


        if (IsKeyPressed(KEY_F))
        {
            ToggleFullscreen();
        }

        if (IsKeyPressed(KEY_R)) {
            toggleRain = !toggleRain;
        }

        // Toggle rain logic to reset rain drops
        if (!toggleRain && previousRain) {
            ResetActiveRainDrops();
        }
        previousRain = toggleRain;
        // =====================================

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
        0.0f);                         // Move to target (zoom)

        //Updating grass patch
        UpdateGrassPatches(camera.target);

        lightShaderUpdate(camera, light);
        //Updating Ground
        UpdateGroundPatches(camera.target);
        BeginDrawing();
            ClearBackground(BLACK);
            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                    DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);

                } break;
                case TITLE:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                    DrawText("PRESS ENTER to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    BeginMode3D(camera);

                BeginShaderMode(light);
                rlDisableBackfaceCulling();

                rlDisableDepthMask();
                   DrawModel(skybox, (Vector3){0,0,0},20.0f,BLACK);
                rlEnableDepthMask();

                //Draw Grass
                //DrawGrassNew(grass);

                //Draw Rain
                if (toggleRain) DrawRain(rain, camera.target);
                else ResetActiveRainDrops();
                
                rlEnableBackfaceCulling();     
                //Draw Ground with backface culling
                DrawGround(ground);

                //Draw Fireflies
                DrawFireflies(firefly, camera.target);

            EndMode3D();

                } break;
                default: break;
            }
        EndDrawing();
    }
    UnloadModel(grass);
    UnloadModel(ground);
    UnloadModel(firefly);
    UnloadModel(rain);
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox);

    CloseWindow();

    return 0;
}