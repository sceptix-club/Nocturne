#include <raylib.h>
#include <rcamera.h>
#include "rlgl.h"
#include "raymath.h" 
#include "world/skybox.h"


#include "world/ground.h"

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "ARTEMIS");

    Camera camera = {0};
    camera.position = (Vector3){ -15.0f, 2.0f, 2.0f };
    camera.target = (Vector3){ 0.185f, 1.5f, -1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    int cameraMode = CAMERA_FIRST_PERSON;

    //Model ground = Ground();
    Model skybox = skybox(); //Skybox
    // This is the 32x32 plane that's below the player
    Model ground = Ground();

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        if (IsKeyPressed(KEY_F))
        {
            ToggleFullscreen();
        }

        // time += GetFrameTime();
        // This is needed for updating the grass animation
        // float bendFactor = TODO: Update with time

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advance camera controls, it's reecommended to compute camera movement manually
        UpdateCamera(&camera, cameraMode);                  // Update camera

/*
        // Camera PRO usage example (EXPERIMENTAL)
        // This new camera function allows custom movement/rotation values to be directly provided
        // as input parameters, with this approach, rcamera module is internally independent of raylib inputs
        UpdateCameraPro(&camera,
            (Vector3){
                (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,    
                (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                0.0f                                                // Move up-down
            },
            (Vector3){
                GetMouseDelta().x*0.05f,                            // Rotation: yaw
                GetMouseDelta().y*0.05f,                            // Rotation: pitch
                0.0f                                                // Rotation: roll
            },
            GetMouseWheelMove()*2.0f);                              // Move to target (zoom)
*/

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);

                DrawModel(ground, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, (Color){255, 255, 255, 255});

            EndMode3D();

            // Draw info boxes
            // DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
            // DrawRectangleLines(5, 5, 330, 100, BLUE);

            // DrawText("Camera controls:", 15, 15, 10, BLACK);
            // DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
            // DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
            // DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
            // DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
            // DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

            // DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
            // DrawRectangleLines(600, 5, 195, 100, BLUE);

            // DrawText("Camera status:", 610, 15, 10, BLACK);
            // DrawText(TextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" :
            //                                   (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
            //                                   (cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
            //                                   (cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 30, 10, BLACK);
            // DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
            //                                         (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
            // DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
            // DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
            // DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}