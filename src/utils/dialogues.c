#include "utils/dialogues.h"
#include "utils/ui.h"

bool isMusicLoaded = false;
bool Dialogue1played = false;
bool Dialogue2played = false;
Sound softType;  

//Each Dialogue is to be defined here
Dialogue firstDialogue[5] = {
    {3.27, "It takes a long time to realize how truly miserable you are."},
    {3.11, "And even longer to see that it doesn't have to be that way."},
    {2.77, "Only after you give up everything,"},
    {1.25, "can you begin?"},
    {2.5, "to find your way to be happy."}
};

Dialogue opening[3] = {
    {15,"What happened ?"},
    {20,"Where am I ?"},
    {50, "Why can't I remember anything ?"}
};

Dialogue findSomething[2] = {
    {3, "I guess..."},
    {10, "I'm supposed to find something ..."}
};

float duration = 0.0f;
int dialogueIndex = 0;
int textWidth,textHeight;
int playStart;



//Same function could be used for drawing different dialogue sets
//TO-DO : Font syles
void DrawSubtitle(Sound audio, bool play, Dialogue *dialogue, int numDialogues, float totalDuration) {
    if (play) {

        if (duration == 0.0f) {
            PlaySound(audio);
        }

        if (duration <= totalDuration) {
            duration += GetFrameTime();  // Increment the timer

            textWidth = MeasureText(dialogue[dialogueIndex].dialogue, 30);
            textHeight = 30;
            DrawText(dialogue[dialogueIndex].dialogue, (sW - textWidth) / 2, (sH - textHeight - 120), 30, WHITE);

            if (duration >= dialogue[dialogueIndex].duration) {
                duration = 0.0f;  // Reset the duration timer
                dialogueIndex++;   // Move to the next dialogue
                StopSound(audio);
            }
        }
    }
}


void Dialogue1(bool play)
{
    if(play && !Dialogue1played)
    {
        if(!isMusicLoaded)
        {
            softType = LoadSound("assets/dialogues/softtype.mp3");
            isMusicLoaded = true;
            dialogueIndex = 0;
        }

        DrawSubtitle(softType, true, opening, sizeof(opening) / sizeof(opening[0]),90);
        if(dialogueIndex >= 3 )
        {
            Dialogue1played = true;
            duration = 0;
            dialogueIndex = 0;
        }
    }
}

void Dialogue2(bool play)
{
    if(play && !Dialogue2played && Dialogue1played)
    {
        DrawSubtitle(softType, true, findSomething, sizeof(findSomething) / sizeof(findSomething[0]),30);
        if(dialogueIndex >= 2 )
        {
            Dialogue2played = true;
            duration = 0;
            dialogueIndex = 0;
        }
    }
}