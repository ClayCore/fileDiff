#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif /* WIN32_LEAN_AND_MEAN */

#ifndef _UNICODE
    #define _UNICODE
#endif /* _UNICODE */

#include <windows.h>

int CALLBACK WinMain(HINSTANCE hi, HINSTANCE hprev, LPSTR cmdline, int show)
{
    UNREFERENCED_PARAMETER(hi);
    UNREFERENCED_PARAMETER(hprev);
    UNREFERENCED_PARAMETER(cmdline);
    UNREFERENCED_PARAMETER(show);

    MessageBox(0, "hello, world!", "fileDiffGUI", MB_OK);

    return 0;
}
