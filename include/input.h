#ifndef INPUT_H
#define INPUT_H

/* Logical keys polled with Input_IsKeyHeld/Input_PressedOnce (sysInputIsKeyHeld). Keys 0-13
   are the bindable actions (gPadBindings); 14 and up are fixed pad buttons. The directions
   also accept the stick past +/-0x28. */
#define KEY_UP 14
#define KEY_DOWN 15
#define KEY_LEFT 16
#define KEY_RIGHT 17
#define KEY_A 18
#define KEY_B 19
#define KEY_C_UP 20
#define KEY_C_DOWN 21
#define KEY_Z 22
#define KEY_R 23
#define KEY_C_LEFT 24
#define KEY_C_RIGHT 25
#define KEY_START 26
#define KEY_A_OR_START 27

#endif
