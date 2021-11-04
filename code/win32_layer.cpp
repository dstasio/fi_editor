// win32_layer.cpp
#include <windows.h>
#include "datatypes.h"

global b32 global_running;
global b32 global_error;
global u32 global_width = 1280;
global u32 global_height = 720;

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
        global_running = true;
        MSG message    = {};

        while(global_running)
        {
            while(PeekMessageA(&message, main_window, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
    }
    else
    {
        // @todo: logging
        return 1;
    }

    return 0;
}
