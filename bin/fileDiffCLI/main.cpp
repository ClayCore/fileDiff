#include "platform/win32/win32.h"

#include <iostream>

int main()
{
    auto stat = fdlib_stat_file("private_data\\test.txt");

    std::cout << stat.name << " | " << stat.namelen << "\n";

    return 0;
}
