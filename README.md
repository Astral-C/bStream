# bStream - A Binary Reader/Writer for C++
bStream is a header only library for file io in c++ that wraps an `std::fstream` to allow for easily reading and writing many common data types.

## Usage
To include bStream in your project simply put 
```cpp
#define BSTREAM_IMPLEMENTATION
``` 
in _one_ of the files where you are including bStream. Alternatively you can simply add bstream.cpp to your project's files and it will handle this for you.

Once you have bStream included you simply open a `CFileStream` specifying an endianess and open mode (defaults to OpenMode::In), ex
```
bStream::CFileStream file("path/to/file.bin", bStream::Endianess::Big, bStream::OpenMode::In);
``` 
or 
```
bStream::CFileStream file = new bStream::CFileStream("path/to/file.bin", bStream::Endianess::Big, bStream::OpenMode::In);
```
