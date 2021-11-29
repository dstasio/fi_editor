// win32_renderer_software.cpp
#include "win32_renderer_software.h"

#define pixel_at_index(mem, x, y, w) (((u32*)(mem)) + ((y) * (w)) + (x))

internal void sw_draw_square(Renderer_Backbuffer *backbuffer,
                 v2i pos, u32 size)
{
    v2i tl = {}; // top-left
    v2i br = {}; // bottom-right
    tl.x = max(pos.x, 0);
    tl.y = max(pos.y, 0);
    br.x = min(pos.x + size, backbuffer->width);
    br.y = min(pos.y + size, backbuffer->height);

    for (s32 x = tl.x; x < br.x; ++x) {
    for (s32 y = tl.y; y < br.y; ++y)
        {
            u32 pixel = 0xFFFFFF;
            *pixel_at_index(backbuffer->memory, x, y, backbuffer->width) = pixel;
        }
    }
}

internal void sw_clear_backbuffer(Renderer_Backbuffer *backbuffer)
{
    SecureZeroMemory(backbuffer->memory, backbuffer->width*backbuffer->height*4);
}

internal void sw_show_backbuffer(HWND window, Renderer_Backbuffer *backbuffer)
{
    HDC device_context = GetDC(window);
    StretchDIBits(device_context,
                  0, 0, backbuffer->width, backbuffer->height,
                  0, 0, backbuffer->width, backbuffer->height,
                  backbuffer->memory, &backbuffer->info,
                  DIB_RGB_COLORS, SRCCOPY);
    ReleaseDC(window, device_context);
}

internal void sw_resize_backbuffer(Renderer_Backbuffer *backbuffer, u32 width, u32 height)
{
    backbuffer->width                        = width;
    backbuffer->height                       = height;
    backbuffer->info.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    backbuffer->info.bmiHeader.biWidth       = width;
    backbuffer->info.bmiHeader.biHeight      = height;
    backbuffer->info.bmiHeader.biPlanes      = 1;
    backbuffer->info.bmiHeader.biBitCount    = 32;
    backbuffer->info.bmiHeader.biCompression = BI_RGB;
    //backbuffer->info.bmiHeader.biXPelsPerMeter = ; // @todo: Raymond Chen says this is unused (by GDI), check if it has any meaning for dpi-awareness
    //backbuffer->info.bmiHeader.biYPelsPerMeter = ;
    CreateDIBSection(0, &backbuffer->info, DIB_RGB_COLORS, &backbuffer->memory, 0, 0);
    fi_assert(backbuffer->memory);

}
