#pragma once
#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <cassert>

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
	OpenMode mode;
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

	CFileStream(std::string, Endianess, OpenMode mod = OpenMode::In);
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

#if defined(BSTREAM_IMPLEMENTATION)
namespace bStream {

uint32_t swap32(uint32_t r){
	return ( ((r>>24)&0xFF) | ((r<<8) & 0xFF0000) | ((r>>8)&0xFF00) | ((r<<24)&0xFF000000));
}

uint16_t swap16(uint16_t r){
	return ( ((r<<8)&0xFF00) | ((r>>8)&0x00FF) );
}

Endianess getSystemEndianess(){
	union {
		uint32_t integer;
		uint8_t bytes[sizeof(uint32_t)];
	} check;
	check.integer = 0x01020304U;
	return (check.bytes[0] == 0x01 ? Endianess::Big : Endianess::Little);
}

//TODO: Clean this garbo up
CFileStream::CFileStream(std::string path, Endianess ord, OpenMode mod){
	base.open(path, std::ios::in | std::ios::out | std::ios::binary);
	filePath = path;
	order = ord;
	mode = mod;
	systemOrder = getSystemEndianess();
}

std::fstream &CFileStream::getStream(){
	return base;
}

std::string CFileStream::getPath(){
	return filePath;
}


void CFileStream::seek(long pos){
	base.seekg(pos, base.beg);
}

long CFileStream::tell(){
	return base.tellg();
}

uint32_t CFileStream::readUInt32(){
	assert(mode == OpenMode::In);
	uint32_t r;
	base.read((char*)&r, sizeof(uint32_t));
	if(order != systemOrder){
		return swap32(r);
	}
	else{
		return r;
	}
}

int32_t CFileStream::readInt32(){
	assert(mode == OpenMode::In);
	int32_t r;
	base.read((char*)&r, sizeof(int32_t));
	if(order != systemOrder){
		return swap32(r);
	}
	else{
		return r;
	}
}

uint16_t CFileStream::readUInt16(){
	assert(mode == OpenMode::In);
	uint16_t r;
	base.read((char*)&r, sizeof(uint16_t));
	if(order != systemOrder){
		return swap16(r);
	}
	else{
		return r;
	}
}

int16_t CFileStream::readInt16(){
	assert(mode == OpenMode::In);
	int16_t r;
	base.read((char*)&r, sizeof(int16_t));
	if(order != systemOrder){
		return swap16(r);
	}
	else{
		return r;
	}
}

uint8_t CFileStream::readUInt8(){
	assert(mode == OpenMode::In);
	uint8_t r;
	base.read((char*)&r, sizeof(uint8_t));
	return r;
}

int8_t CFileStream::readInt8(){
	assert(mode == OpenMode::In);
	int8_t r;
	base.read((char*)&r, sizeof(int8_t));
	return r;
}

//todo: think of how to clean this up
float CFileStream::readFloat(){
	assert(mode == OpenMode::In);
	char buff[sizeof(float)];
	base.read(buff, sizeof(float));
	if(order != systemOrder){
		char temp[sizeof(float)];
		temp[0] = buff[3];
		temp[1] = buff[2];
		temp[2] = buff[1];
		temp[3] = buff[0];
		return *((float*)temp);
	}
	return *((float*)buff);
}

char* CFileStream::readBytes(size_t size){
	assert(mode == OpenMode::In);
	char* buffer = new char[size];
	base.read(buffer, size);
	return buffer;
}

std::string CFileStream::readString(size_t len){
	assert(mode == OpenMode::In);
    std::string str(len, '\0'); //creates string str at size of length and fills it with '\0'
    base.read(&str[0], len);
    return str;
}

std::string CFileStream::readWString(size_t len){
	assert(mode == OpenMode::In);
    std::string str(len, '\0'); //creates string str at size of length and fills it with '\0'
    base.read(&str[0], len);
    return str;
}

void CFileStream::writeInt8(int8_t v){
	assert(mode == OpenMode::Out);
	base.write((char*)&v, 1);
}

void CFileStream::writeUInt8(uint8_t v){
	assert(mode == OpenMode::Out);
	base.write((char*)&v, 1);
}

void CFileStream::writeInt16(int16_t v){
	assert(mode == OpenMode::Out);
	if(order != systemOrder){
		v = swap16(v);
	}
	base.write((char*)&v, sizeof(uint16_t));
}

void CFileStream::writeUInt16(uint16_t v){
	assert(mode == OpenMode::Out);
	if(order != systemOrder){
		v = swap16(v);
	}
	base.write((char*)&v, sizeof(uint16_t));
}

void CFileStream::writeInt32(int32_t v){
	assert(mode == OpenMode::Out);
	if(order != systemOrder){
	   v = swap32(v);
	}
	base.write((char*)&v, sizeof(int32_t));
}

void CFileStream::writeUInt32(uint32_t v){
	assert(mode == OpenMode::Out);
	if(order != systemOrder){
	   v = swap32(v);
	}
	base.write((char*)&v, sizeof(uint32_t));
}

void CFileStream::writeFloat(float v){
	assert(mode == OpenMode::Out);
	char* buff = (char*)&v;
	if(order != systemOrder){
		char temp[sizeof(float)];
		temp[0] = buff[3];
		temp[1] = buff[2];
		temp[2] = buff[1];
		temp[3] = buff[0];
		v = *((float*)temp);
	}
	base.write((char*)&v, sizeof(float));
}

void CFileStream::writeString(std::string v){
	assert(mode == OpenMode::Out);
	base.write(v.c_str(), v.size());
}

void CFileStream::writeBytes(char* v, size_t size){
	assert(mode == OpenMode::Out);
	base.write(v, size);
}

void CFileStream::readStruct(void* out, size_t size){
	assert(mode == OpenMode::In);
	base.read((char*)out, size);
}

uint8_t CFileStream::peekU8(int offset){
	assert(mode == OpenMode::In);
	uint8_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt8();
	base.seekg(pos, base.beg);
	return ret;
}

int8_t CFileStream::peekI8(int offset){
	assert(mode == OpenMode::In);
	int8_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt8();
	base.seekg(pos, base.beg);
	return ret;
}

uint16_t CFileStream::peekU16(int offset){
	assert(mode == OpenMode::In);
	uint16_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt16();
	base.seekg(pos, base.beg);
	return ret;
}

int16_t CFileStream::peekI16(int offset){
	assert(mode == OpenMode::In);
	int16_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt16();
	base.seekg(pos, base.beg);
	return ret;
}

uint32_t CFileStream::peekU32(int offset){
	assert(mode == OpenMode::In);
	uint32_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt32();
	base.seekg(pos, base.beg);
	return ret;
}

int32_t CFileStream::peekI32(int offset){
	assert(mode == OpenMode::In);
	int32_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt32();
	base.seekg(pos, base.beg);
	return ret;
}

size_t CFileStream::getSize(){
	int pos = base.tellg();
	base.seekg(0, std::ios::end);
	size_t ret = base.tellg();
	base.seekg(pos, std::ios::beg);
	return ret;
}

CMemoryStream::CMemoryStream(uint8_t* ptr, size_t size, Endianess ord){
	mBuffer = ptr;
	mPosition = 0;
	mSize = size;
	order = ord;
	systemOrder = getSystemEndianess();
}

size_t CMemoryStream::getSize(){
	return mSize;
}

void CMemoryStream::seek(size_t pos){
	mPosition = (pos > mSize ? mPosition : pos);
}

//TODO: Make it so that you cant read past the end of the stream

int8_t CMemoryStream::readInt8(){
	int8_t r;
	memcpy(&r, OffsetPointer<int8_t>(mBuffer, mPosition), sizeof(int8_t));
	mPosition++;
	return r;
}

uint8_t CMemoryStream::readUInt8(){
	uint8_t r;
	memcpy(&r, OffsetPointer<uint8_t>(mBuffer, mPosition), sizeof(uint8_t));
	mPosition++;
	return r;
}

int16_t CMemoryStream::readInt16(){
	int16_t r;
	memcpy(&r, OffsetPointer<int16_t>(mBuffer, mPosition), sizeof(int16_t));
	mPosition += sizeof(int16_t);

	if(order != systemOrder){
		return swap16(r);
	}
	else{
		return r;
	}
}

uint16_t CMemoryStream::readUInt16(){
	uint16_t r;
	memcpy(&r, OffsetPointer<uint16_t>(mBuffer, mPosition), sizeof(uint16_t));
	mPosition += sizeof(uint16_t);

	if(order != systemOrder){
		return swap16(r);
	}
	else{
		return r;
	}
}

uint32_t CMemoryStream::readUInt32(){	
	uint32_t r;
	memcpy(&r, OffsetPointer<uint32_t>(mBuffer, mPosition), sizeof(uint32_t));
	mPosition += sizeof(uint32_t);

	if(order != systemOrder){
		return swap32(r);
	}
	else{
		return r;
	}
}

int32_t CMemoryStream::readInt32(){
	int32_t r;
	memcpy(&r, OffsetPointer<int32_t>(mBuffer, mPosition), sizeof(int32_t));
	mPosition += sizeof(int32_t);

	if(order != systemOrder){
		return swap32(r);
	}
	else{
		return r;
	}
}

std::string CMemoryStream::readString(size_t len){
	std::string str('\0', len);
	strncpy(&str[0], OffsetPointer<char>(mBuffer, mPosition), len);
	mPosition += len;
	return str;
}

}
#endif