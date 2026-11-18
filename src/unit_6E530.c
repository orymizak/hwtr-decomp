#include "common.h"
#include "include_asm.h"
s32 Snd_FadeOutAll(s32,s32);
s32 nuScRemoveClient(void *);
extern s8 gAudioTaskEnabled;
extern u8 D_80144450;

/* Voice groups for Snd_FadeOutAll (bit mask). */
#define SND_GROUP_SFX 1 /* voices playing sound effects */
#define SND_GROUP_SEQ 2 /* voices owned by the music sequencer */

/* Audio task events. 2 fades every voice out; 1 with 0x1C stops the task and removes its
   scheduler client. TODO: name the event ids once their senders are known. */
void AudioTask_HandleEvent(s16 arg0, s32 arg1) {
    switch (arg0) {
    case 2:
        Snd_FadeOutAll(SND_GROUP_SEQ, 0x19);
        Snd_FadeOutAll(SND_GROUP_SFX, 0x19);
        return;
    case 1:
        if (arg1 == 0x1C) {
            gAudioTaskEnabled = 0;
            nuScRemoveClient(&D_80144450);
        }
        return;
    }
}
