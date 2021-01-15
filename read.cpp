#include <iostream>
#include <fstream>
#include <string>
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

int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " FILE" << endl;
    return -1;
  }

  tensorflow::Example example;

  {
    // Read the existing file.
    fstream input(argv[1], ios::in | ios::binary);
    if (!example.ParseFromIstream(&input)) {
      cerr << "Failed to parse file." << endl;
      return -1;
    } else {
      cout << "Succeed in parsing file." << endl;
    }
  }

  ListEntry(example);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
