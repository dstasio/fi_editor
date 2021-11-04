// win32_layer.cpp
#include <windows.h>

int WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_flag)
{
    MessageBox(0, "Hello, Win32!", "fi", MB_OK|MB_ICONINFORMATION);
    return 0;
}
