#include "utils/cutscene.h"
#include <raylib.h>
#include <stdio.h>
#define CUTSCENE_PARTS 4

Image cutsceneBg[CUTSCENE_PARTS];
Texture2D cutsceneBgTex[CUTSCENE_PARTS];
unsigned int cutsceneNextFrameOffset = 0;
int cutsceneAnimFrames[CUTSCENE_PARTS], cutsceneCurrentFrame = 0, cutsceneFrameDelay = 4 , cutsceneFrameCounter = 0,partCounter=0;
int cutsceneTotalFrames = 197;
float elapsed_time =0.0f, start_time;
int length;

void initCutscene()
{
    for(int i=0; i<CUTSCENE_PARTS; i++)
    {
        cutsceneBg[i] = LoadImageAnim(TextFormat("assets/cutscene/finalcutscene%d.gif",i+1), &cutsceneAnimFrames[i]);
        cutsceneBgTex[i] = LoadTextureFromImage(cutsceneBg[i]);
    }
    length = ((GetMonitorHeight(GetCurrentMonitor())-cutsceneBgTex[0].height*2))/2;
}

int PlayCutScene(bool play, Music audio)
{
    if (play)
    {
        if (!IsMusicStreamPlaying(audio))
        {
            PlayMusicStream(audio);
            start_time = GetTime();
        }

        cutsceneFrameCounter++;

        if (cutsceneFrameCounter >= cutsceneFrameDelay)
        {
            cutsceneCurrentFrame++;

            if (cutsceneCurrentFrame >= cutsceneAnimFrames[partCounter])
            {
                cutsceneCurrentFrame = 0;
                partCounter++;
            }

            if (partCounter < CUTSCENE_PARTS)
            {
                if(partCounter == 2) cutsceneFrameDelay = 320;
                else cutsceneFrameDelay = 4;
                cutsceneNextFrameOffset = cutsceneBg[partCounter].width * cutsceneBg[partCounter].height * 4 * cutsceneCurrentFrame;
                UpdateTexture(cutsceneBgTex[partCounter], ((unsigned char*)cutsceneBg[partCounter].data) + cutsceneNextFrameOffset);
            }

            cutsceneFrameCounter = 0;
        }

        if (partCounter < CUTSCENE_PARTS)
        {
            DrawTextureEx(cutsceneBgTex[partCounter], (Vector2){0, length}, 0, 2, WHITE);
        }
    }

    if ((elapsed_time = GetTime() - start_time )> 53.0f)
    {
        StopMusicStream(audio);
        return 1; //Stop playing cutscene
    }
    return 0;  // Still playing cutscene
}
