#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "example.pb.h"
using namespace std;

struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};

void print_hex(char* buf, int len) {
    for(int i = 0; i < len; ++i) {
        cout << setfill('0') << setw(2) << hex << int(buf[i]) << " " ;
        if(i && (i % 10 == 0))
            cout << endl;
    }
    cout << endl;
}

int main(int argc, char* argv[]) {


    if (argc != 2) {
        cerr << "Usage:  " << argv[0] << " FILE_PATH" << endl;
        return -1;
    }

    // Read the existing file.
    fstream input(argv[1], ios::in | ios::binary);

    // Read length
    uint64_t length;
    input.read(reinterpret_cast<char *>(&length), 8);
    cout << length << endl;

    // Read length crc
    uint32_t crc_length;
    input.read(reinterpret_cast<char *>(&crc_length), 4);
    cout << "crc_length: " << hex << crc_length << endl;

    // Read data
    char data_buffer[length+1];
    data_buffer[length] = '\0';
    input.read(data_buffer, length);
    print_hex(data_buffer, length);
    
    // Read data crc
    uint32_t crc_data;
    input.read(reinterpret_cast<char *>(&crc_data), 4);
    cout << "crc_data: " << hex << crc_data << endl;

    membuf sbuf(data_buffer, data_buffer + length);
    std::istream in(&sbuf);

    tensorflow::Example example;
    if (!example.ParseFromIstream(&in)) {
        cerr << "Failed to parse file." << endl;
        return -1;
    } else {
        cout << "successfully parse file" << endl;
    }


    input.close();
    return 0;
}
