#include "vfs/file.h"

#include <cstdio>

int main()
{
    auto *file = fdlib_file_new(13, "res/data.txt");

    std::puts("fileDiffCLI");

    fdlib_file_destroy(file);

    return 0;
}
