#include "utils/dialogues.h"

//Each Dialogue is to be defined here
Dialogue firstDialogue[5] = {
    {3.27, "It takes a long time to realize how truly miserable you are."},
    {3.11, "And even longer to see that it doesn't have to be that way."},
    {2.77, "Only after you give up everything,"},
    {1.25, "can you begin?"},
    {2.5, "to find your way to be happy."}
};

float duration = 0.0f;
int dialogueIndex = 0;
int screenW,screenH,textWidth,textHeight;


//Same function could be used for drawing different dialogue sets
//TO-DO : Font syles
void DrawSubtitle(Music audio, bool play, Dialogue *dialogue, int numDialogues) {
    if (play) {
        if (IsKeyPressed(KEY_B))
            PlayMusicStream(audio);

        if (dialogueIndex < numDialogues && IsMusicStreamPlaying(audio)) {
            duration += GetFrameTime();

            screenW = GetMonitorWidth(GetCurrentMonitor());
            screenH = GetMonitorHeight(GetCurrentMonitor());
            textWidth = MeasureText(dialogue[dialogueIndex].dialogue, 30);
            textHeight = 30;

            DrawText(dialogue[dialogueIndex].dialogue, (screenW-textWidth)/2, (screenH-textHeight-50), 30, WHITE);

            if (duration >= dialogue[dialogueIndex].duration) {
                duration = 0.0f;
                dialogueIndex++;
            }
        }
    }
}
