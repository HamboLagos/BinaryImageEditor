#include <fstream>

#include "cli_utils.inl"

int main(int argc, char *argv[])
{
    std::string progName = argv[0];
    if(argc != 2) {
        fail(progName, "no image file specified");
    }

    // open and load the binary image
    std::string fileName = argv[1];
    std::ifstream raw_image(fileName, std::ios::in);
    if(!raw_image || raw_image.bad()) {
        fail(progName, std::string("unable to open image file: ") + argv[1]);
    }

    std::cout << "Hello World" << std::endl;
    return 0;
}
