#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "example.pb.h"
using namespace std;

// Iterates though all feature in the example and print them.
void ListEntry(const tensorflow::Example& example) {
    auto features = example.features();
    int feature_size = features.feature_size(); 
    cout << "total feature number: " << features.feature_size() << endl;
    
    // all feature is const ::PROTOBUF_NAMESPACE_ID::Map<std::string, ::tensorflow::Feature>
    auto all_feature_map = features.feature();
    for (google::protobuf::Map< std::string, tensorflow::Feature >::const_iterator it = all_feature_map.begin(); it != all_feature_map.end(); ++it) {
        cout << "---------------------------" << endl;
        auto cur_feature_name = it->first;
        cout << cur_feature_name << endl;
        auto cur_feature_value = it->second; // is class tensorflow::Feature

        // print bytes list
        if(cur_feature_value.has_bytes_list()) {
            cout << "all bytes values: " << endl;
            auto bytes_list = cur_feature_value.bytes_list();
            for (int i = 0; i < bytes_list.value_size(); ++i)
                cout << bytes_list.value(i) << ", " ;
            cout << endl;
        }

        // print int64 list
        if(cur_feature_value.has_int64_list()) {
            cout << "all int64 values: " << endl;
            auto int64_list = cur_feature_value.int64_list();
            for (int i = 0; i < int64_list.value_size(); ++i)
                cout << int64_list.value(i) << ", " ;
            cout << endl;
        }

        // print float list
        if(cur_feature_value.has_float_list()) {
            cout << "all float values: " << endl;
            auto float_list = cur_feature_value.float_list();
            for (int i = 0; i < float_list.value_size(); ++i)
                cout << float_list.value(i) << ", " ;
            cout << endl;
        }
    }
}

struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};

void print_hex(char* buf, int len) {
    buf[len] = '\0';
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

    // Read the file.
    fstream input(argv[1], ios::in | ios::binary);

    // repeatedly Read length
    uint64_t length;
    while(input.read(reinterpret_cast<char *>(&length), 8)) {
        cout << "=======================" << endl;

        cout << "current example length: " << length << endl;

        // Read length crc
        uint32_t crc_length;
        input.read(reinterpret_cast<char *>(&crc_length), 4);
        // cout << "crc_length: " << hex << crc_length << endl;

        // Read data
        char data_buffer[length];
        input.read(data_buffer, length);
        // print_hex(data_buffer, length);
        
        // Read data crc
        uint32_t crc_data;
        input.read(reinterpret_cast<char *>(&crc_data), 4);
        // cout << "crc_data: " << hex << crc_data << endl;

        membuf sbuf(data_buffer, data_buffer + length);
        std::istream in(&sbuf);

        tensorflow::Example example;
        if (!example.ParseFromIstream(&in)) {
            cerr << "Failed to parse file." << endl;
            return -1;
        } else {
            // cout << "successfully parse file" << endl;
        }

        ListEntry(example);
    }

    input.close();
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
