#include "types/aliases.hpp"

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif /* WIN32_LEAN_AND_MEAN */

#ifndef _UNICODE
    #define _UNICODE
#endif /* _UNICODE */

#ifndef UNICODE
    #define UNICODE
#endif /* UNICODE */

#include <windows.h>

auto CALLBACK WinMain(HINSTANCE hi, HINSTANCE hprev, LPSTR cmdline, int show) -> i32
{
    UNREFERENCED_PARAMETER(hi);
    UNREFERENCED_PARAMETER(hprev);
    UNREFERENCED_PARAMETER(cmdline);
    UNREFERENCED_PARAMETER(show);

    MessageBoxA(nullptr, "hello, world!", "fileDiffGUI", MB_OK);

    return 0;
}
