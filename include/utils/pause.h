#ifndef PAUSE_H
#define PAUSE_H

#include <raylib.h>

Image imOrigin;
Texture2D screen;
Color *pixels;
Rectangle screen_size;

// Function to capture the current screen and apply effects before pausing
void GetCurrentScreen()
{
    // Capture the screen
    imOrigin = LoadImageFromScreen(); // Correct function name, capture the current screen image
    ImageFormat(&imOrigin, PIXELFORMAT_UNCOMPRESSED_GRAYSCALE); // Convert to the required format
    ImageBlurGaussian(&imOrigin,10);
    screen = LoadTextureFromImage(imOrigin);
    screen_size.x = 10;
    screen_size.y = 10;
    screen_size.width = (float)GetMonitorWidth(GetCurrentMonitor());
    screen_size.height = (float)GetMonitorHeight(GetCurrentMonitor());
}

void DrawPause()
{
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureRec(screen,screen_size,(Vector2){0,0},WHITE);
        DrawText("GAME PAUSED",10,10,50,WHITE);
    EndDrawing();
}

#endif
