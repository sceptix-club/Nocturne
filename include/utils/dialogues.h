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
void DrawSubtitle(Music audio, bool play, Dialogue *dialogue, int numDialogues);

#endif
