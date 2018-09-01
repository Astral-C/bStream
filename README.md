# bStream - A Binary Reader/Writer for C++
bStream is a header only library for file io in c++ that wraps an `std::fstream` to allow for easily reading and writing many common data types.

## Usage
To include bStream in your project simply put 
```cpp
#define BSTREAM_IMPLEMENTATION
#include "bstream.h"
``` 
in _one_ of the files where you are including bStream. Alternatively you can simple add bstream.cpp to your projects files and it will handle this for you.

Once you have bStream included you simple open a `CFileStream` specifying an endianess and open mode (defaults to in if not specified), ex
```
bStream::CFileStream file("path/to/file.bin", bStream::Endianess::Big, bStream::OpenMode::In);
``` 
or 
```
bStream::CFileStream file = new bStream::CFileStream("path/to/file.bin", bStream::Endianess::Big, bStream::OpenMode::In);
```

## TODO
Currently open mode isnt properly set and all streams are read write. This is not intended (there is meant to be seperate read streams and write streams) however to properly overwrite parts of a file you must use the in, out, and binary flags for opening an fstream. This will be fixed in the future and it is reccomended that you design your programs with seperate read and write streams in mind, if you do not your code may no longer work once this is fixed.