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
	std::string file_path;
	int stream_pos;
	int last_pos;
	Endianess order;


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
	void seek(long);
	std::string getPath();

	char peekI8(int);
	short peekI16(int);
	int peekI32(int);

    std::fstream &getStream();

	unsigned char peekU8(int);
	unsigned short peekU16(int);
	unsigned int peekU32(int);

    bStream(std::string, Endianess, int rw = 0);
	bStream() {}
	~bStream() {this->base.close();}
};
