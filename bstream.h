#pragma once
#include <iostream>
#include <cstdint>
#include <fstream>

enum Endianess{
	Big, Little
};

class bStream {
private:
	std::fstream base;
	std::string filePath;
	int anchor;
	int lastPos;
	Endianess order;
	Endianess systemOrder;

public:

	Endianess getSystemEndianess();
	void readStruct(void*, size_t);

	//read functions
	int8_t readInt8();
	uint8_t readUInt8();
	int16_t readInt16();
	uint16_t readUInt16();
	int32_t readInt32();
	uint32_t readUInt32();
	float readFloat();
	char* readBytes(int);
	std::string readString(int);

	//write functions
	void writeInt8(int8_t);
	void writeUInt8(uint8_t);
	void writeInt16(int16_t);
	void writeUInt16(uint16_t);
	void writeInt32(int32_t);
	void writeUInt32(uint32_t);
	void writeFloat(float);
	void writeString(std::string);

	//utility functions
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

	bStream(std::string, Endianess, int rw = 0);
	bStream() {}
	~bStream() {this->base.close();}
};
