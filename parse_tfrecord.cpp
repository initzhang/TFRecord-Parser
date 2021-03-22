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
    uint64_t length = 0;
    uint64_t max_len = 1000;
    char* data_buffer = new char[max_len];
    
    while(input.read(reinterpret_cast<char *>(&length), 8)) {
        if(length > max_len) {
            delete data_buffer;
            max_len = 2 * length;
            data_buffer = new char[max_len];
        }
        cout << "=======================" << endl;
        cout << "current example length: " << length << endl;

        // the following fields: crc of length(4 bytes), data(length bytes), crc of data(4 bytes)
        // we directly read in (length+8) bytes, and skip the head/tail
        input.read(data_buffer, length + 8);

        /*
        print_hex(data_buffer, 4); // print crc of length
        print_hex(data_buffer + 4, length); // print data
        print_hex(data_buffer + length + 4, 4); // print crc of data
        */
        
        tensorflow::Example example;
        if (!example.ParseFromArray(reinterpret_cast<void*>(data_buffer + 4), length)) {
            cerr << "Failed to parse file." << endl;
            return -1;
        } else {
            cout << "successfully parse file into an Example" << endl;
        }

        ListEntry(example);
    }

    input.close();
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
