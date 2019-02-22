#include <iostream>
#include <string>

#include "analyse.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Error, no command provided" << std::endl;
        return 1;
    }

    if (auto command = std::string_view(argv[1]); command == "analyse")
    {
        analyse(argc, argv);
    }
    else
    {
        return 2;
    }
}
