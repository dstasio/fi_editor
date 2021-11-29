// win32_layer.cpp
#include <windows.h>
#include "datatypes.h"

// @todo: internal only
#define fi_assert(x) if(!(x)) {*(int *)0 = 0;}

#define byte_offset(arr, t)  ((u8*)(arr) + (t))

#define array_count(arr) (sizeof(arr)/sizeof((arr)[0]))

#include "fi_platform.h"
#include "win32_layer.h"
#include "win32_renderer_software.cpp"

global b32 global_running;
global b32 global_error;
global u32 global_width  = 1000;
global u32 global_height = 720;
global r32 global_ar = (r32)global_width / (r32)global_height;

LRESULT CALLBACK
win32_callback_proc(HWND window, UINT message, WPARAM w, LPARAM l)
{
    LRESULT result = 0;
    switch(message)
    {
        case WM_CLOSE:
        {
            DestroyWindow(window);
        } break;

        case WM_DESTROY:
        {
            global_running = false;
            PostQuitMessage(0);
        } break;

        case WM_SIZE:
        {
            global_width  = LOWORD(l);
            global_height = HIWORD(l);
        } break;

        // @todo: implement this
        case WM_SETCURSOR:
        {
        } break;

        default:
        {
            result = DefWindowProcA(window, message, w, l);
        } break;
    }
    return(result);
}

int WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_flag)
{
    WNDCLASSA fi_window_class     = {};
    fi_window_class.style         = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    fi_window_class.lpfnWndProc   = win32_callback_proc;
    fi_window_class.hInstance     = instance;
    fi_window_class.lpszClassName = "fi_window_class";
    ATOM Result                   = RegisterClassA(&fi_window_class);

    RECT window_dims = {0, 0, (s32)global_width, (s32)global_height};
    AdjustWindowRect(&window_dims, WS_OVERLAPPEDWINDOW, FALSE);
    window_dims.right  -= window_dims.left;
    window_dims.bottom -= window_dims.top;

    HWND main_window = CreateWindowA("fi_window_class", "fi",
                                    WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                    CW_USEDEFAULT, CW_USEDEFAULT,
                                    window_dims.right,
                                    window_dims.bottom,
                                    0, 0, instance, 0);
    if (main_window)
    {
        Fi_State state = {};
        state.cursor_size = 50;
        state.cursor_pos = {170, 250};

        global_running = true;
        MSG message    = {};

        // Creating DIB section, to use as backbuffer for window
        // ======================================================================
        Renderer_Backbuffer backbuffer = {};
        sw_resize_backbuffer(&backbuffer, global_width, global_height);

        Input input_buffer[2];
        Input *new_input = &input_buffer[0];
        Input *old_input = &input_buffer[1];
        while(global_running)
        {
            *new_input = {};
            while(PeekMessageA(&message, main_window, 0, 0, PM_REMOVE))
            {
                switch(message.message)
                {
                case WM_KEYDOWN:
                {
                    for (Input_Key *key = &new_input->move_down;
                         key < &new_input->last; ++key)
                    {
                        if (message.wParam == key->scancode) {
                            key->down = 1;
                            break;
                        }
                    }
                } break;
                case WM_KEYUP:
                {
                } break;
                default:
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                } break;
                }
            }

            if (new_input->move_down.down)
                state.cursor_pos.y -= 1;
            if (new_input->move_up.down)
                state.cursor_pos.y += 1;
            if (new_input->move_left.down)
                state.cursor_pos.x -= 1;
            if (new_input->move_right.down)
                state.cursor_pos.x += 1;

            sw_clear_backbuffer(&backbuffer);

            // drawing cursor
            sw_draw_square(&backbuffer, state.cursor_pos, state.cursor_size);

            sw_show_backbuffer(main_window, &backbuffer);

            Input *temp_input = new_input;
            new_input = old_input;
            old_input = temp_input;
        }
    }
    else
    {
        // @todo: logging
        return 1;
    }

    return 0;
}
