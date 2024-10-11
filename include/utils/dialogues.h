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
extern bool DialoguePlayed[6];

// Function declaration
void DrawSubtitle(Sound audio, bool play, Dialogue *dialogue, int numDialogues, float duration);


//Dialogue methods
void Dialogue1(bool play);
void Dialogue2(bool play);
void Dialogue3(bool play);
void Dialogue4(bool play);
void Dialogue5(bool play);
void Dialogue6(bool play);
#endif
