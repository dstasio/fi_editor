#if !defined(FI_PLATFORM_H)

#define SCANCODE_COUNT 0x100
#include "win32_platform.h"

struct Input_Key
{
    b32 down;
    u32 n_transitions;
};

struct Keymap {
    // @optimize: maybe use indices instead of pointers, should save memory
    //            but then you'd have to take care of non-initialized values (which will have to be different than 0)
    //            or use a dummy command for the 0th index
    u8 scancodes[SCANCODE_COUNT];
};

struct Input
{
    Input_Key _first;
    Input_Key move_down;
    Input_Key move_up;
    Input_Key move_left;
    Input_Key move_right;
    Input_Key _last;
};
#define command_index(input, cmdname) ((u8)((&((input)->cmdname)) - (&((input)->_first))))

#define FI_PLATFORM_H
#endif
