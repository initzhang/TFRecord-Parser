read:
	g++ -std=c++11 read_message.cpp example.pb.cc feature.pb.cc -o read `pkg-config --cflags --libs protobuf`
write:
	g++ -std=c++11 write_message.cpp example.pb.cc feature.pb.cc -o write `pkg-config --cflags --libs protobuf`
parse:
	g++ -std=c++11 parse_tfrecord.cpp example.pb.cc feature.pb.cc -o parse `pkg-config --cflags --libs protobuf`
	
