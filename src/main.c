
#include <raylib.h>
#include <rcamera.h>
#include <stdio.h>
#include <rlgl.h>
#include <raymath.h>

#define RLIGHTS_IMPLEMENTATION

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#include "shaders/lights.h"
#include "shaders/rlights.h"
#include "world/skybox.h"
#include "world/ground.h"
#include "world/objects.h"
#include "world/grass.h"
#include "world/firefly.h"
#include "world/props.h"
#include "world/rain.h"
#include "utils/pause.h"
#include "utils/dialogues.h"
#include "stdio.h"

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, PAUSE} GameScreen;

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;
    GameScreen currentScreen = LOGO;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "ARTEMIS");
    int framesCounter = 0;

    Camera camera = {0};
    camera.position = (Vector3){ 0.0f, 5.0f, 0.0f };
    camera.target = (Vector3){ 5.0f, 5.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 75.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Model ground = Ground();
    Model skybox = SkyBox();

    // Light above camera
    Shader light = SetLights();

    // display cam position
    Model ground = GroundModel(light);

    // Get the object model
    Model object = ObjectModel(light);
    Model marker = MarkerModel();

    //  grass model
    Model grass = GrassBladeModel(light);

    // firefly model
    Model firefly = FireflyModel();

    // rain model
    Model rain = RainModel();

    Model rubble = Bone(light);

    //Initialize grass
    InitGrass(camera.target);

    //Initialize Ground
    InitGround(camera.target);

    // Initialise hidden objects
    InitObjects();

    //Initialize Fireflies
    InitFireflies(camera.target);

    //Initialize Rain
    InitRain(camera.target);

    InitAudioDevice();

    //Empty texture for cinamtic shader
    Shader cinematic = Cinematic();
    RenderTexture2D cTexture = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor())); // Bloom overlay.
    Texture2D pause_screen;
    Image screen;
    Music firstAudio = LoadMusicStream("assets/dialogues/testAudio.mp3");
    DisableCursor();
    SetTargetFPS(60);

    bool toggleRain = false;
    bool previousRain = false;

    // --------------------------------------------------------------------------------------

    while (!WindowShouldClose()) // Gameloop
    {
        UpdateMusicStream(firstAudio);
        switch(currentScreen)
        {
            case LOGO:
            {
                framesCounter++;    // Count frames
                if (framesCounter > 120)
                {
                    currentScreen = TITLE;
                }
            } break;
            case TITLE:
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                
            } break;
            default: break;
        }

        if (IsKeyPressed(KEY_F))
        {
            ToggleFullscreen();
        }

        if (IsKeyPressed(KEY_R)) {
            toggleRain = !toggleRain;
        };

        // Toggle rain logic to reset rain drops
        if (toggleRain && !previousRain) {
            ResetActiveRainDrops();
            InitRain(camera.target);
        }
        previousRain = toggleRain;

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

        UpdateLightShader(camera, light);

        // Begin drawing
        BeginDrawing();
        BeginTextureMode(cTexture);
        ClearBackground(BLACK);
        switch(currentScreen)
        {
            case LOGO: {
                DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
            } 
            break;

            case TITLE: {
                DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                DrawText("PRESS ENTER to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
            } 
            break;

            case GAMEPLAY: {
                BeginMode3D(camera);

                BeginShaderMode(light);
                rlDisableBackfaceCulling();
                rlDisableDepthMask();
                DrawModel(skybox, (Vector3){0,0,0}, 20.0f, BLACK);
                rlEnableDepthMask();

                // Random objects
                DrawObjects(object, &camera);
                DrawMarkers(marker);

                //Draw Grass
                DrawGrass(grass, camera.target);

                if (toggleRain) {
                    DrawRain(rain, camera.target);
                }

                rlEnableBackfaceCulling();     
                DrawGround(ground, camera.target);
                DrawFireflies(firefly, camera.target);
                DrawBone(true,Vector3Zero());

                EndMode3D();
                
                if(IsKeyPressed(KEY_P))
                {
                    GetCurrentScreen();
                    currentScreen = PAUSE;
                }
                
                // Check if all objects are found
                // Placeholder for now
                if (allObjectsFound) {
                    puts("YOU FOUND ALL OBJECTS!");
                }

            } 
            break;

            case PAUSE: {
                DrawPause();
                if(IsKeyPressed(KEY_L)) {
                    currentScreen = GAMEPLAY;
                }
            }
            break;

            default: break;
        }

        EndTextureMode();

        BeginShaderMode(cinematic);
        DrawTextureRec(cTexture.texture, (Rectangle){0, 0, cTexture.texture.width, -cTexture.texture.height}, (Vector2){0, 0}, BLANK);
        EndShaderMode();

        DrawSubtitle(firstAudio, true, firstDialogue, sizeof(firstDialogue) / sizeof(firstDialogue[0]));

        EndDrawing();
    }

    UnloadShader(light);
    UnloadModel(grass);
    UnloadModel(ground);
    UnloadModel(firefly);
    UnloadModel(rain);
    UnloadModel(object);
    UnloadModel(marker);
    UnloadShader(cinematic);
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox);

    CloseWindow();

    return 0;
}

