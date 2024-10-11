#include "utils/dialogues.h"
#include "utils/ui.h"

bool isMusicLoaded = false;
bool DialoguePlayed[6]={false,false,false,false,false,false};
Sound softType;  

//Each Dialogue is to be defined here
// Dialogue firstDialogue[5] = {
//     {3.27, "It takes a long time to realize how truly miserable you are."},
//     {3.11, "And even longer to see that it doesn't have to be that way."},
//     {2.77, "Only after you give up everything,"},
//     {1.25, "can you begin?"},
//     {2.5, "to find your way to be happy."}
// };

//Dialogue 1
Dialogue opening[3] = {
    {15,"What happened ?"},
    {20,"Where am I ?"},
    {50, "Why can't I remember anything ?"}
};

//Dialogue 2
Dialogue findSomething[2] = {
    {3, "I guess..."},
    {10, "I'm supposed to find something ..."}
};

//Dialogue 3
Dialogue foundBone[3] = {
    {6, "A bone?!"},
    {3,"That's weird"},
    {5,"Bones in the middle of nowhere..."}
};

//Dialogue 4
Dialogue foundBall[2] = {
    {7, "A ball? Why does this seem familiar?..."},
    {5, "That's... strange."}
};

//Dialogue 5
Dialogue foundMap[4] = {
    {3, "A map?!"},
    {4, "Something looks off about this "},
    {5, "It's better than nothing i guess..."},
    {8, "This has to lead somewhere"}
};

//Dialogue 6
Dialogue foundGrave[3] = {
    {3, "A grave?!"},
    {3, "OK THIS IS GETTING WEIRD"},
    {3, "WHAT'S HAPPENING TO ME?!"}
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
    if(play && !DialoguePlayed[0])
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
            DialoguePlayed[0] = true;
            duration = 0;
            dialogueIndex = 0;
        }
    }
}

void Dialogue2(bool play)
{
    if(play && !DialoguePlayed[1] && DialoguePlayed[0])
    {
        DrawSubtitle(softType, true, findSomething, sizeof(findSomething) / sizeof(findSomething[0]),30);
        if(dialogueIndex >= 2 )
        {
            DialoguePlayed[1] = true;
            duration = 0;
            dialogueIndex = 0;
        }
    }
}

void Dialogue3(bool play)
{
    if(play && !DialoguePlayed[2] && DialoguePlayed[1])
    {
        DrawSubtitle(softType, true, foundBone, sizeof(foundBone) / sizeof(foundBone[0]),20);
        if(dialogueIndex >= 3 )
        {
            DialoguePlayed[2] = true;
            duration = 0;
            dialogueIndex = 0;
        }
    }
}

void Dialogue4(bool play)
{
    if(play && !DialoguePlayed[3] && DialoguePlayed[2])
    {
        DrawSubtitle(softType, true, foundBall, sizeof(foundBall) / sizeof(foundBall),20);
        if(dialogueIndex >= 2 )
        {
            DialoguePlayed[3] = true;
            duration = 0;
            dialogueIndex = 0;
        }
    }
}

void Dialogue5(bool play)
{
    if(play && !DialoguePlayed[4] && DialoguePlayed[3])
    {
        DrawSubtitle(softType, true, foundMap, sizeof(foundMap) / sizeof(foundMap[0]), 20);
        if(dialogueIndex >= 4 )
        {
            DialoguePlayed[4] = true;
            duration = 0;
            dialogueIndex = 0;
        }
    }
}

void Dialogue6(bool play)
{
    if(play && !DialoguePlayed[5] && DialoguePlayed[4])
    {
        DrawSubtitle(softType, true, foundGrave, sizeof(foundGrave) / sizeof(foundGrave[0]), 20);
        if(dialogueIndex >= 3 )
        {
            DialoguePlayed[5] = true;
            duration = 0;
            dialogueIndex = 0;
        }
    }
}