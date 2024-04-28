#include "platform/os.h"
#include "types/aliases.hpp"

#include <iostream>

auto main() -> i32
{
    auto stat = fdlib_stat_file("private_data\\test.txt");

    std::cout << stat.name << " | " << stat.namelen << "\n";

    return 0;
}
