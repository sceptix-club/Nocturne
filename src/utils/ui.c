#include <raylib.h>
#include <utils/ui.h>

Image nocturne;
Image raylib;
Texture2D logo,raylib_logo;
int sW,sH;
Rectangle playButton,optionsButton,exitButton;

void InitUI()
{
    nocturne = LoadImage("assets/logo.png");
    raylib = LoadImage("assets/raylib_logo.png");
    
    logo = LoadTextureFromImage(nocturne);
    raylib_logo = LoadTextureFromImage(raylib);
    sW = GetMonitorWidth(GetCurrentMonitor());
    sH = GetMonitorHeight(GetCurrentMonitor());

    UnloadImage(nocturne);
    UnloadImage(raylib);

    playButton = (Rectangle){BUTTON_MARGIN, sH - 5 * (BUTTON_HEIGHT + 30), BUTTON_WIDTH, BUTTON_HEIGHT};
    optionsButton = (Rectangle){BUTTON_MARGIN, sH - 4 * (BUTTON_HEIGHT + 30), BUTTON_WIDTH, BUTTON_HEIGHT};
    exitButton = (Rectangle){BUTTON_MARGIN, sH - 3* (BUTTON_HEIGHT + 30), BUTTON_WIDTH, BUTTON_HEIGHT};

}

void DrawRaylib()
{
    BeginDrawing();
        float posX = (sW - raylib_logo.width) / 2.0f;
        float posY = (sH - raylib_logo.height) / 2.0f;
        DrawRectangle(0, 0, sW, sH, WHITE);
        DrawTextureEx(raylib_logo, (Vector2){ posX, posY }, 0.0f, 1.0f, WHITE);
        int textWidth = MeasureText("THE SCEPTIX CLUB",20);
        DrawText("THE SCEPTIX CLUB",(sW-textWidth)/2,(sH-60),20,BLACK);
        DrawText("WAIT for a few SECONDS...", 20, sH-60, 20, GRAY);
    EndDrawing();

}

void DrawLoadingScreen()
{
    BeginDrawing();
        float posX = (sW - logo.width) / 2.0f;
        float posY = (sH - logo.height) / 2.0f;
        DrawRectangle(0, 0, sW, sH, BLACK);
        DrawTextureEx(logo, (Vector2){ posX, posY }, 0.0f, 1.0f, WHITE);
        DrawText("PRESS ENTER to GAMEPLAY SCREEN", 120, 220, 20, WHITE);
    EndDrawing();
}

ButtonClicked CheckClick(Vector2 mousePos) {

        if (CheckCollisionPointRec(mousePos, playButton)&& IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                return PLAY_BUTTON;
        }
        if (CheckCollisionPointRec(mousePos, optionsButton)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                return OPTIONS_BUTTON;
            }
        }
        if (CheckCollisionPointRec(mousePos, exitButton)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                return EXIT_BUTTON;
            }
        }

    return NO_BUTTON;
}

void DrawUI(bool draw)
{
  if(draw)
  {
        DrawText("PLAY", playButton.x + 20, playButton.y + 10, 20, WHITE);
        DrawText("OPTIONS", optionsButton.x + 20, optionsButton.y + 10, 20, WHITE);
        DrawText("EXIT", exitButton.x + 20, exitButton.y + 10, 20, WHITE);

        DrawRectangleLines(playButton.x, playButton.y, playButton.width, playButton.height, RED);
        DrawRectangleLines(optionsButton.x, optionsButton.y, optionsButton.width, optionsButton.height, RED);
        DrawRectangleLines(exitButton.x, exitButton.y, exitButton.width, exitButton.height, RED);  
  }
   
}
