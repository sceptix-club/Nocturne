#ifndef DIALOGUE_H
#define DIALOGUE_H
#include <raylib.h>
#define MAX_TEXT_LENGTH 100

typedef struct {
    float duration;
    char dialogue[MAX_TEXT_LENGTH];
} Dialogue;

// Declare the variables as extern
extern Dialogue firstDialogue[5];
extern float duration;
extern int dialogueIndex;

// Function declaration
void DrawSubtitle(Sound audio, bool play, Dialogue *dialogue, int numDialogues, float duration);


//Dialogue methods
void Dialogue1(bool play);
void Dialogue2(bool play);
void Dialogue3(bool play);

#endif
