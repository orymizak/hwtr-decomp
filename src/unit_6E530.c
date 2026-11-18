#include "common.h"
#include "include_asm.h"
s32 Snd_FadeOutAll(s32,s32);
s32 nuScRemoveClient(void *);
extern s8 gAudioTaskEnabled;
extern u8 D_80144450;

    /* TODO (Clean version):
    nuScRemoveClient is osScRemoveClient(&sc, &audioClient) and Snd_FadeOutAll triggers voice fadeout:
    void audioHandleEvent(s16 eventType, s32 param) {
        switch (eventType) {
        case AUDIO_EVENT_STOP_ALL: // 2
            audioFadeOutVoices(VOICE_GROUP_SFX, 25);
            audioFadeOutVoices(VOICE_GROUP_SEQ, 25);
            return;
        case AUDIO_EVENT_SHUTDOWN: // 1
            if (param == AUDIO_SHUTDOWN_PARAM) { // 0x1C
                audioEnabled = 0;
                osScRemoveClient(&sc, &audioClient);
            }
            return;
        }
    }
    */

    void AudioTask_HandleEvent(s16 arg0, s32 arg1) {
    switch (arg0) {
    case 2:
        Snd_FadeOutAll(2, 0x19);
        Snd_FadeOutAll(1, 0x19);
        return;
    case 1:
        if (arg1 == 0x1C) {
            gAudioTaskEnabled = 0;
            nuScRemoveClient(&D_80144450);
        }
        return;
    }
}
