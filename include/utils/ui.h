#ifndef UI_H
#define UI_H
#include <raylib.h>
#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 40
#define BUTTON_MARGIN 200



typedef enum {
    NO_BUTTON,
    PLAY_BUTTON,
    OPTIONS_BUTTON,
    EXIT_BUTTON
} ButtonClicked;



void InitUI();
void DrawRaylib();
void DrawLoadingScreen();
void DrawUI(bool draw);
ButtonClicked CheckClick(Vector2 mousePos);

#endif