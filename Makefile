read:
	g++ -std=c++11 read.cpp example.pb.cc feature.pb.cc -o read `pkg-config --cflags --libs protobuf`
write:
	g++ -std=c++11 write.cpp example.pb.cc feature.pb.cc -o write `pkg-config --cflags --libs protobuf`
parse:
	g++ -std=c++11 strip_parse.cpp example.pb.cc feature.pb.cc -o parse `pkg-config --cflags --libs protobuf`
	
