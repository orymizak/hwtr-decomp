#include "common.h"
#include "include_asm.h"
s32 func_800CE228(s32,s32);
s32 nuScRemoveClient(void *);
extern s8 D_800FABA0;
extern u8 D_80144450;

    /* TODO (Clean version):
    nuScRemoveClient is osScRemoveClient(&sc, &audioClient) and func_800CE228 triggers voice fadeout:
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

    void func_800CD930(s16 arg0, s32 arg1) {
    switch (arg0) {
    case 2:
        func_800CE228(2, 0x19);
        func_800CE228(1, 0x19);
        return;
    case 1:
        if (arg1 == 0x1C) {
            D_800FABA0 = 0;
            nuScRemoveClient(&D_80144450);
        }
        return;
    }
}
