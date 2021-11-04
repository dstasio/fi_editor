// win32_layer.cpp
#include <windows.h>
#include "datatypes.h"

#define fi_assert(x) if(!(x)) {*(int *)0 = 0;}

global b32 global_running;
global b32 global_error;
global u32 global_width  = 1000;
global u32 global_height = 1000;

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

        // Creating DIB section, to use as backbuffer for window
        // ======================================================================
        BITMAPINFO dib_info = {};
        dib_info.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
        dib_info.bmiHeader.biWidth         = global_width;
        dib_info.bmiHeader.biHeight        = global_height;
        dib_info.bmiHeader.biPlanes        = 1;
        dib_info.bmiHeader.biBitCount      = 32;
        dib_info.bmiHeader.biCompression   = BI_RGB;
        //dib_info.bmiHeader.biXPelsPerMeter = ; // @todo: Raymond Chen says this is unused (by GDI), check if it has any meaning for dpi-awareness
        //dib_info.bmiHeader.biYPelsPerMeter = ;

        void *dib_memory = 0;
        CreateDIBSection(0, &dib_info, DIB_RGB_COLORS, &dib_memory, 0, 0);
        fi_assert(dib_memory);

        for (u32 x = 0; x < global_width; ++x)
        {
            for (u32 y = 0; y < global_height; ++y)
            {
                u32 cell_size = 500;
                u8 r = 0xFF;
                u8 g = (u8)((x % cell_size) * (global_width  / (cell_size - 1)));
                u8 b = (u8)((y % cell_size) * (global_height / (cell_size - 1)));
                u32 pixel = (r << 16) | (g << 8) | b;
                *(((u32*)dib_memory)+y*global_width+x) = pixel;
            }
        }

        HDC device_context = GetDC(main_window);
        StretchDIBits(device_context,
                      0, 0, global_width, global_height,
                      0, 0, global_width, global_height,
                      dib_memory, &dib_info, DIB_RGB_COLORS, SRCCOPY);

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
