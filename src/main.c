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
#include "world/rain.h"
#include "world/props.h"
#include "utils/pause.h"
#include "utils/dialogues.h"
#include "utils/ui.h"
#include "utils/cutscene.h"
#include "stdio.h"

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, PAUSE, LOBBY, OPTIONS, EXIT, CUTSCENE} GameScreen;

int main(void)
{
    const int screenWidth = GetMonitorWidth(GetCurrentMonitor());
    const int screenHeight = GetMonitorHeight(GetCurrentMonitor());
    GameScreen currentScreen = LOGO;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Nocturne");
    ToggleFullscreen();
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
    AllObjects object = ObjectModel(light);
    Model marker = MarkerModel();
    //  grass model
    Model grass = GrassBladeModel(light);
    // firefly model
    Model firefly = FireflyModel();
    // rain model
    Model rain = RainModel(light);
    // Model rubble = Bone(light);
    Model tree = MakeTree(light);

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
    SetAudioStreamBufferSizeDefault(8192);
    InitUI();
    initCutscene();
    InitTress(camera.target);

    //Empty texture for cinamtic shader
    Shader cinematic = Cinematic();
    // RenderTexture2D cTexture = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor())); // Bloom overlay.
    RenderTexture2D cutscenetexture = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
    RenderTexture2D lobbyTexture = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
    RenderTexture2D gameplayTexture = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));

    Music firstAudio = LoadMusicStream("assets/dialogues/testAudio.mp3");
    Music bgm = LoadMusicStream("assets/thelongdark.mp3");
    Music cutsceneaudio = LoadMusicStream("assets/cutsceneAudio.mp3");


    cutsceneaudio.stream.sampleRate = 44100;
    bgm.stream.sampleRate = 44100;
    // SetMusicPitch(bgm,1.0f);
    // DisableCursor();
    SetTargetFPS(60);

    bool toggleRain = false;
    bool toggleBgm = false;
    bool toggleCutScene = false;
    bool previousRain = false;


    printf("Loaded sequence: ");
    for (unsigned int i = 0; i < 4; i++) {
        printf("%d ", seq[i]);
    }
    printf("\n");

    // --------------------------------------------------------------------------------------

    while (!WindowShouldClose()) // Gameloop
    {
        UpdateMusicStream(cutsceneaudio);
        UpdateMusicStream(firstAudio);
        UpdateMusicStream(bgm);
        switch(currentScreen)
        {
            case LOGO:
            {
                framesCounter++;    // Count frames
                if (framesCounter > 40)
                {
                    currentScreen = TITLE;
                    framesCounter=0;
                }
            } break;
            case TITLE:
            {
                framesCounter++;
                if (framesCounter > 60)
                {
                    currentScreen = LOBBY;
                }
            } break;
            case LOBBY:
            {
                    BeginTextureMode(lobbyTexture);
                    ClearBackground(BLACK);
                    ButtonClicked choice = CheckClick(GetMousePosition());
                    switch(choice)
                    {
                        case PLAY_BUTTON:
                        {
                            DisableCursor();                        
                            currentScreen = GAMEPLAY;
                        }
                        break;
                        case OPTIONS_BUTTON:
                        {
                            currentScreen = OPTIONS;
                        }
                        break;
                        case EXIT_BUTTON:
                        {
                            currentScreen = EXIT;
                        }
                        break;
                        default :
                        break;
                    }
                    DrawUI(true);
                    EndTextureMode();
            }
            break;

            case GAMEPLAY:
            {
                  // DisableCursor();
                if (IsKeyPressed(KEY_F))
                {
                    ToggleFullscreen();
                }

                if (IsKeyPressed(KEY_R)) {
                    toggleRain = !toggleRain;
                }
                if (IsKeyPressed(KEY_C))
                {
                    currentScreen = CUTSCENE;
                    if(!toggleCutScene)
                        toggleCutScene = !toggleCutScene;
                }
                if(IsKeyPressed(KEY_M)){
                    toggleBgm = !toggleBgm;
                    if(toggleBgm)
                        PlayMusicStream(bgm);
                    else
                        StopMusicStream(bgm);
                }
                // Toggle rain logic to reset rain drops
                if (!toggleRain && previousRain) {
                    ResetActiveRainDrops();
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
                BeginTextureMode(gameplayTexture);
                ClearBackground(BLACK);
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
                //Draw Trees
                DrawTrees(tree,camera.target);
                if (toggleRain) {
                    DrawRain(rain, camera.target);
                }
                rlEnableBackfaceCulling();     
                DrawGround(ground, camera.target);
                DrawFireflies(firefly, camera.target);
                EndMode3D();
                if(IsKeyPressed(KEY_P))
                {
                    GetCurrentScreen();
                    currentScreen = PAUSE;
                }
                if (allObjectsFound) {
                    puts("YOU FOUND ALL OBJECTS!");
                }
                EndShaderMode();
                EndTextureMode();
            } break;

            case CUTSCENE:
            {
                BeginTextureMode(cutscenetexture);
                ClearBackground(BLACK);
                int end = PlayCutScene(true, cutsceneaudio);
                if (end)
                {
                    StopMusicStream(cutsceneaudio);
                    currentScreen = GAMEPLAY;

                }
                EndTextureMode();
            }break;
        }


        // Begin drawing
        switch(currentScreen)
        {
            case LOGO: {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawRaylib();
                EndDrawing();
            } 
            break;

            case TITLE: {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawLoadingScreen();
                EndDrawing();
            } 
            break;
            
            case LOBBY:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTextureRec(lobbyTexture.texture, (Rectangle){ 0, 0, lobbyTexture.texture.width, -lobbyTexture.texture.height }, (Vector2){ 0, 0 }, WHITE);
                EndDrawing();
            }
            break;

            case GAMEPLAY: {
                BeginDrawing();
                ClearBackground(BLACK);
                BeginShaderMode(cinematic);
                DrawTextureRec(gameplayTexture.texture, (Rectangle){ 0, 0, gameplayTexture.texture.width, -gameplayTexture.texture.height }, (Vector2){ 0, 0 }, WHITE);
                EndShaderMode();
                DrawMovieFrame();
                DrawSubtitle(firstAudio, true, firstDialogue, sizeof(firstDialogue) / sizeof(firstDialogue[0]));
                EndDrawing();
            } 
            break;

            case CUTSCENE:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTextureRec(cutscenetexture.texture, (Rectangle){0,0,cutscenetexture.texture.width, -cutscenetexture.texture.height}, (Vector2){0,0},WHITE);
                EndDrawing();
            } break;

            case PAUSE: {
                DrawPause();
                if(IsKeyPressed(KEY_L)) {
                    currentScreen = GAMEPLAY;
                }
            }
            break;

            case EXIT:
            {
                CloseWindow();
            }
            break;
        }

        // EndTextureMode();
    }

    UnloadShader(light);
    UnloadModel(grass);
    UnloadModel(ground);
    UnloadModel(firefly);
    UnloadModel(rain);
    UnloadModel(tree);
    UnloadModel(object.bone);
    UnloadModel(object.ball);
    UnloadModel(object.sign);
    UnloadModel(object.grave);

    UnloadModel(marker);
    free(seq);
    
    UnloadShader(cinematic);
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox);

    UnloadRenderTexture(cutscenetexture);
    UnloadRenderTexture(lobbyTexture);
    UnloadRenderTexture(gameplayTexture);
    
    CloseWindow();

    return 0;
}

