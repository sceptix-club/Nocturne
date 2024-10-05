#include "utils/cutscene.h"
#include <raylib.h>
#include <stdio.h>

Image cutsceneBg;
Texture2D cutsceneBgTex;
unsigned int cutsceneNextFrameOffset = 0;
int cutsceneAnimFrames = 0, cutsceneCurrentFrame = 0, cutsceneFrameDelay =4 , cutsceneFrameCounter = 0;
int cutsceneTotalFrames = 1590;
int length;

void initCutscene()
{
    cutsceneBg = LoadImageAnim("assets/cutscene/finalcutscene1.gif", &cutsceneAnimFrames);
    // printf("cutsceneAnimeFrames : %d",cutsceneAnimFrames);
    cutsceneBgTex = LoadTextureFromImage(cutsceneBg);
    length = (GetMonitorHeight(GetCurrentMonitor())-cutsceneBgTex.height)/2;
}

int PlayCutScene(bool play, Music audio)
{
    if(play)
    {
        if(!IsMusicStreamPlaying(audio))
            PlayMusicStream(audio);
        printf("Cutscene Should be processing rn\n");
        cutsceneFrameCounter++;
        if(cutsceneFrameCounter >= cutsceneFrameDelay)
        {
            cutsceneCurrentFrame++;
            if(cutsceneCurrentFrame >= cutsceneAnimFrames)
            {
                cutsceneCurrentFrame = 0;
            }
            cutsceneNextFrameOffset = cutsceneBg.width * cutsceneBg.height * 4 * cutsceneCurrentFrame;
            UpdateTexture(cutsceneBgTex, ((unsigned char*)cutsceneBg.data) + cutsceneNextFrameOffset);
            cutsceneFrameCounter = 0;
        }
        DrawTextureEx(cutsceneBgTex,(Vector2){0,0},0,2,WHITE);
    }
    
    if(cutsceneCurrentFrame == cutsceneTotalFrames) return 1;
    else return 0;
}
