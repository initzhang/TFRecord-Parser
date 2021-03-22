# TFRecord-Parser
TFRecord parser using C++ and Protocal Buffer 

## Requirements
protoc 3.8.0 & C++ 11

## Usage

```
make parse
./parse samples/one_sample_from_py.tfr # can parse any tfrecord file, print the content to stdout

make read
./read samples/single_message # can only read file containing a Serialized Example without header and footer

make write
./write <your new file> # the result file is simply a Serialized Example Message without header and footer

```

## Background about this project

I want to parse TFRecord file using C++, after some searching I found the [tensorflow C++ TFRecordReader API document](https://www.tensorflow.org/api_docs/cc/class/tensorflow/ops/t-f-record-reader). But to use this API, you need to compile and build tensorflow C++ from source, which is not easy and too complicated for such a simple need. So I open this project to decode TFRecord using C++ directly.

To use and understand this project, you need to have some basic knowledge about [Protocal Buffer](https://developers.google.com/protocol-buffers/docs/overview).

## TFRecord file's organization

[TFRecord's Official document](https://www.tensorflow.org/tutorials/load_data/tfrecord) explains that TFRecord is composed of some `tf.train.Example`, which is exactly a `message` of protobuf. The definition about the `message` lies in the file `example.proto` in the tensorflow code base (current [link](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/example/example.proto) to the file).

For the purpose of checking and validation, TFRecord also add header and footer to each `tf.train.Example`, information about these addtional fields can be found in `record_writer.h` (current [link](https://github.com/tensorflow/tensorflow/blob/516ae286f6cc796e646d14671d94959b129130a4/tensorflow/core/lib/io/record_writer.h) to this file). Below shows the most important lines:

```
class RecordWriter {
 public:
  // Format of a single record:
  //  uint64    length
  //  uint32    masked crc of length
  //  byte      data[length]
  //  uint32    masked crc of data
  static constexpr size_t kHeaderSize = sizeof(uint64) + sizeof(uint32);
  static constexpr size_t kFooterSize = sizeof(uint32);
```

From the comments above we can see that, the `tf.train.Example` is stored in the data field, the length field store the bytes length, and two crc fields are added respectively.

So now it is clear how we can parse one TFRecord file:
1. read a TFRecord in binary format
2. read the first 8 bytes into a uint64, get the length
3. skip the 4 bytes crc of length (**you should be careful about this**)
4. read length bytes as data, and use Protocal Buffer C++ API to decode it.
5. skip the 4 bytes crc of data
6. repeat 2~5 until you reach the end of the file

## Protocal Buffer

To decode the `data` field, you need corresponding Protocal Buffer C++ API, so you need to compile proto files first. Note that the `feature.proto` is imported in `example.proto`, so you need to compile both and reference corresponding C++ header afterwards(when compile your own read/write C++ code).

The `feature.proto` and the `example.proto` is downloaded from tensorflow-r1.15 code base. The corresponding protoc version is 3.8.0.

To use a different version of Protoc, just `protoc --cpp_out=. feature.proto` and `protoc --cpp_out=. example.proto`

For a single protobuf message, sample read and write usage is in `read_message.cpp` and `write_message.cpp`.

For tfrecord, sample parse usage is in `parse_tfrecord.cpp`, this file can parse arbitrary TFRecord files.



