#pragma once

#include <iostream>
#include <cstdlib>

inline void usage(std::string name, std::string reason)
{
    std::string usage("usage:");
    usage += "\n\t" + name + " {image_file}";

    std::cout
        << reason << std::endl
        << usage << std::endl;
}

inline void fail(std::string name, std::string reason)
{
    usage(name, reason);
    exit(-1);
}
