#if !defined(FI_PLATFORM_H)

#include "win32_platform.h"

struct Input_Key
{
    // @todo: find way to map multiple scancodes to one command
    u32 scancode;
    b32 down;
    u32 n_transitions;
};

struct Input
{
    Input_Key move_down  = {KEY_DOWN};
    Input_Key move_up    = {KEY_UP};
    Input_Key move_left  = {KEY_LEFT};
    Input_Key move_right = {KEY_RIGHT};
    Input_Key last;
};

#define FI_PLATFORM_H
#endif
