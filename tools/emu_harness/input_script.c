/* Mando virtual para mupen64plus: devuelve los botones que fija el lanzador con input_script_set(). */
#include <string.h>

typedef struct { int Present; int RawData; int Plugin; int Type; } CONTROL;
typedef struct { CONTROL *Controls; } CONTROL_INFO;

static volatile unsigned int g_keys;

static volatile unsigned int g_reads;

void input_script_set(unsigned int keys) { g_keys = keys; }
unsigned int input_script_reads(void) { return g_reads; }

int PluginGetVersion(int *type, int *version, int *api, const char **name, int *caps) {
    if (type) *type = 4; /* M64PLUGIN_INPUT */
    if (version) *version = 0x010000;
    if (api) *api = 0x020101;
    if (name) *name = "Guion de entrada";
    if (caps) *caps = 0;
    return 0;
}
int PluginStartup(void *lib, void *ctx, void *cb) { return 0; }
int PluginShutdown(void) { return 0; }
void ControllerCommand(int control, unsigned char *command) {}
void GetKeys(int control, unsigned int *keys) {
    if (control == 0) g_reads++;
    *keys = control == 0 ? g_keys : 0;
}
void InitiateControllers(CONTROL_INFO info) {
    int i;
    for (i = 0; i < 4; i++) {
        info.Controls[i].Present = i == 0;
        info.Controls[i].RawData = 0;
        info.Controls[i].Plugin = 1; /* PLUGIN_NONE */
    }
}
void ReadController(int control, unsigned char *command) {}
int RomOpen(void) { return 1; }
void RomClosed(void) {}
void SDL_KeyDown(int mod, int sym) {}
void SDL_KeyUp(int mod, int sym) {}
