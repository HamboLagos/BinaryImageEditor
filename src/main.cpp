#include <fstream>
#include <vector>

#include "cli_utils.inl"

int main(int argc, char *argv[])
{
    std::string progName = argv[0];
    if(argc != 2) {
        fail(progName, "no image file specified");
    }

    // open and load the binary image
    std::string fileName = argv[1];
    std::ifstream image_file(fileName, std::ios::in | std::ios::binary);
    if(!image_file || image_file.bad()) {
        fail(progName, std::string("unable to open image file: ") + argv[1]);
    }

    // get the raw data
    constexpr int IMAGE_SIDE_LEN = 256;
    constexpr int PIXELS_PER_BYTE = 8;
    constexpr size_t RAW_IMAGE_SIZE = IMAGE_SIDE_LEN * IMAGE_SIDE_LEN / PIXELS_PER_BYTE;

    std::vector<bool> data; // use false for black, true for white
    data.reserve(RAW_IMAGE_SIZE);

    image_file.seekg(0, std::ios::end);
    size_t file_size = image_file.tellg();

    // there is probably a better way to do this
    size_t header_size = file_size - RAW_IMAGE_SIZE;
    image_file.seekg(header_size, std::ios::beg); // skip over the header
    size_t bytesLeft = RAW_IMAGE_SIZE;
    while(bytesLeft--) {
        using Byte = char;
        Byte buffer[1];
        image_file.read(buffer, 1);

        Byte& nextByte = buffer[0];
        for (int ndx = 0; ndx < PIXELS_PER_BYTE; ++ndx) {
            constexpr uint8_t mask = 0b1000000;
            Byte masked_value = ((mask >> ndx) & nextByte);
            bool is_set = (masked_value != 0);
            data.push_back(is_set);
        }

        if(image_file.fail()) {
            fail(progName, "Error parsing binary image data");
        }
    }

    int ndx = 0;
    for(auto bit : data) {
        if(ndx++ % IMAGE_SIDE_LEN == 0) {
            std::cout << std::endl;
        }

        std::cout << (bit ? "x" : "_");
    }

    std::cout << std::endl;
    return 0;
}
