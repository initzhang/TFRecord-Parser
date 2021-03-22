#include <iostream>
#include <fstream>
#include <string>
#include "example.pb.h"
using namespace std;

int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " FILE_NAME" << endl;
    return -1;
  }

  tensorflow::Example example;

  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
    } else if (!example.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
  }

  cout << "example.has_features(): " << example.has_features() << endl;

  // features: class tensorflow::Features
  // during init it already points to a default Features instance
  auto features = example.mutable_features();
  cout << "features.feature_size(): " << features->feature_size() << endl;
  cout << features->ByteSizeLong() << endl;

  // feature_maps: ::google::protobuf::Map< ::std::string, ::tensorflow::Feature >*
  // https://developers.google.com/protocol-buffers/docs/reference/csharp/class/google/protobuf/collections/map-field-t-key-t-value-
  auto feature_maps = features->mutable_feature();
  cout << "feature_maps->size(): " << feature_maps->size() << endl;
  // feature_maps->insert("feature0", tensorflow::Feature());
  // default_feature: class tensorflow::Feature


  /*
  cout << "create a default Feature" << endl;
  auto default_feature = tensorflow::Feature::default_instance();
  cout << "default_feature.kind_case(): " << default_feature.kind_case() << endl;
  */

  cout << "create a new Feature" << endl;
  auto new_feature_ptr = tensorflow::Feature().New();
  cout << "new_feature_ptr->kind_case(): " << new_feature_ptr->kind_case() << endl;

  cout << "adding values to feature_maps" << endl;

  string tmp_key = "feature0";
  tensorflow::Int64List* tmp_int64_list = tensorflow::Int64List().New();
  int64_t haha = 1;
  tmp_int64_list->add_value(haha);
  new_feature_ptr->set_allocated_int64_list(tmp_int64_list);

  string tmp_key1 = "feature1";
  tensorflow::Int64List* tmp_int64_list1 = tensorflow::Int64List().New();
  tmp_int64_list1->add_value(haha);
  auto new_feature_ptr1 = tensorflow::Feature().New();
  new_feature_ptr1->set_allocated_int64_list(tmp_int64_list1);

  for (int i = 0; i < 1000; ++i) {
    string cur_key = "feature" + to_string(i%4);
    tensorflow::Int64List* cur_int64_list = tensorflow::Int64List().New();
    tmp_int64_list1->add_value(haha);
    auto cur_feature_ptr = tensorflow::Feature().New();
    cur_feature_ptr->set_allocated_int64_list(cur_int64_list);
    (*feature_maps)[cur_key] = (*cur_feature_ptr);
  }
  // (*feature_maps)[tmp_key1] = (*new_feature_ptr);
  // (*feature_maps)[tmp_key] = (*new_feature_ptr1);

  cout << "succeed in adding 1000 values" << endl;


  /*  
   *  Now I want to:
   *  1. construct a Map with values
   *  2. insert it to feature_maps
   *  3. write back to file
   *
   */

  
  //auto int64_ptr = feature.second->mutable_int64_list();
  //int64_ptr->add_value(1);
  // cout << features->feature_size() << endl;

  {
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!example.SerializeToOstream(&output)) {
      cerr << "Failed to write address book." << endl;
      return -1;
    }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
