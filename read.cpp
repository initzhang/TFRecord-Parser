#include <iostream>
#include <fstream>
#include <string>
#include "example.pb.h"
using namespace std;

// Iterates though all people in the AddressBook and prints info about them.
/*
void ListEntry(const trf::EntrySet& entry_set) {
  for (int i = 0; i < entry_set.entry_size(); i++) {
    cout << entry_set.entry(i).feature0() << endl;
    }
}
*/

// Main function:  Reads the entire address book from a file and prints all
//   the information inside.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " FILE" << endl;
    return -1;
  }

  //tensorflow::Example::Features fs;
  tensorflow::Example example;

  {
    // Read the existing file.
    fstream input(argv[1], ios::in | ios::binary);
    if (!example.ParseFromIstream(&input)) {
      cerr << "Failed to parse file." << endl;
      return -1;
    } else {
      cout << "haha" << endl;
    }
  }

  //ListEntry(entry_set);

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
