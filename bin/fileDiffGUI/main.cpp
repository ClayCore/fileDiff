#include <windows.h>

#include "vfs/file.h"

int CALLBACK WinMain(HINSTANCE hi, HINSTANCE hprev, LPSTR cmdline, int show)
{
    UNREFERENCED_PARAMETER(hi);
    UNREFERENCED_PARAMETER(hprev);
    UNREFERENCED_PARAMETER(cmdline);
    UNREFERENCED_PARAMETER(show);

    auto *file = fdlib_file_new(13, "res/data.txt");

    MessageBox(0, file->path, "fileDiffGUI", MB_OK);

    fdlib_file_destroy(file);

    return 0;
}
