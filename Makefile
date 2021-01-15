read:
	g++ -std=c++11 read.cpp example.pb.cc feature.pb.cc -o read `pkg-config --cflags --libs protobuf`
strip:
	g++ -std=c++11 strip_parse.cpp example.pb.cc feature.pb.cc -o parse `pkg-config --cflags --libs protobuf`
	
