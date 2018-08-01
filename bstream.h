#pragma once
#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>

namespace bStream {

uint32_t swap32(uint32_t v);
uint16_t swap16(uint16_t v);

template < typename T >
static inline const T * OffsetPointer(const void * ptr, size_t offs) {
  uintptr_t p = reinterpret_cast<uintptr_t>(ptr);
  p += offs;
  return reinterpret_cast<const T *>(p);
}

enum OpenMode {
	In,
	Out
};

enum Endianess{
	Big, Little
};

Endianess getSystemEndianess();

class CFileStream {
private:
	std::fstream base;
	std::string filePath;
	Endianess order;
	Endianess systemOrder;

public:

	void readStruct(void*, size_t);

	//read functions
	int8_t readInt8();
	uint8_t readUInt8();
	int16_t readInt16();
	uint16_t readUInt16();
	int32_t readInt32();
	uint32_t readUInt32();
	float readFloat();
	char* readBytes(size_t);
	std::string readWString(size_t);
	std::string readString(size_t);

	//write functions
	void writeInt8(int8_t);
	void writeUInt8(uint8_t);
	void writeInt16(int16_t);
	void writeUInt16(uint16_t);
	void writeInt32(int32_t);
	void writeUInt32(uint32_t);
	void writeFloat(float);
	void writeBytes(char*, size_t);
	void writeString(std::string);

	//utility functions
	size_t getSize();
	long tell();
	void seek(long);
	std::string getPath();

	uint8_t peekU8(int);
	uint16_t peekU16(int);
	uint32_t peekU32(int);

	int8_t peekI8(int);
	int16_t peekI16(int);
	int32_t peekI32(int);

	std::fstream &getStream();

	CFileStream(std::string, Endianess, OpenMode mode = OpenMode::In);
	CFileStream() {}
	~CFileStream() {this->base.close();}
};

class CMemoryStream {
	private:
		uint8_t* mBuffer;
		size_t mPosition;
		size_t mSize;
		
		Endianess order;
		Endianess systemOrder;
	
	public:

		size_t getSize();
		
		int8_t readInt8();
		uint8_t readUInt8();

		int16_t readInt16();
		uint16_t readUInt16();

		int32_t readInt32();
		uint32_t readUInt32();

		std::string readString(size_t);

		void seek(size_t pos);

		CMemoryStream(uint8_t* ptr, size_t size, Endianess ord);
		CMemoryStream(){}
		~CMemoryStream(){}

};

}