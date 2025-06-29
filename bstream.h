#pragma once
#ifndef __BSTREAM_H__
#define __BSTREAM_H__

#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <cassert>

namespace bStream {

uint32_t swap32(uint32_t v);
uint16_t swap16(uint16_t v);

template < typename T >
static inline const T * OffsetPointer(const void * ptr, std::size_t offs) {
  uintptr_t p = reinterpret_cast<uintptr_t>(ptr);
  p += offs;
  return reinterpret_cast<const T *>(p);
}

template < typename T >
static inline T * OffsetWritePointer(void * ptr, std::size_t offs) {
  uintptr_t p = reinterpret_cast<uintptr_t>(ptr);
  p += offs;
  return reinterpret_cast<T *>(p);
}

enum OpenMode {
	In,
	Out
};

enum Endianess{
	Big, Little
};

Endianess getSystemEndianess();

class CStream {
	public:
		virtual bool seek(std::size_t, bool = false) = 0;
		virtual void skip(std::size_t) = 0;
		virtual std::size_t tell() = 0;

		virtual std::size_t getSize() = 0;

		virtual uint8_t readUInt8() = 0;
		virtual uint16_t readUInt16() = 0;
		virtual uint32_t readUInt32() = 0;

		virtual int8_t readInt8() = 0;
		virtual int16_t readInt16() = 0;
		virtual int32_t readInt32() = 0;

		virtual float readFloat() = 0;
		virtual void writeFloat(float) = 0;

		virtual double readDouble() = 0;
		virtual void writeDouble(double) = 0;

		virtual int8_t peekInt8(std::size_t) = 0;
		virtual uint8_t peekUInt8(std::size_t) = 0;

		virtual int16_t peekInt16(std::size_t) = 0;
		virtual uint16_t peekUInt16(std::size_t) = 0;

		virtual int32_t peekInt32(std::size_t) = 0;
		virtual uint32_t peekUInt32(std::size_t) = 0;

		virtual void writeInt8(int8_t) = 0;
		virtual void writeUInt8(uint8_t) = 0;

		virtual void writeInt32(int32_t) = 0;
		virtual void writeUInt32(uint32_t) = 0;

		virtual void writeInt16(int16_t) = 0;
		virtual void writeUInt16(uint16_t) = 0;

		virtual void readBytesTo(uint8_t*, std::size_t) = 0;
		virtual void writeBytes(uint8_t*, std::size_t) = 0;

		virtual void alignTo(std::size_t) = 0;

		virtual void writeOffsetAt16(std::size_t) = 0;
		virtual void writeOffsetAt32(std::size_t) = 0;

		virtual void writeString(std::string) = 0;
		virtual std::string peekString(std::size_t, std::size_t) = 0;
		virtual std::string readString(std::size_t) = 0;
};

class CFileStream : public CStream {
private:
	std::fstream base;
	std::string filePath;
	OpenMode mode;
	Endianess order;
	Endianess systemOrder;

public:

	template<typename T>
	T readStruct(){
		assert(mode == OpenMode::In);
		T out;
		base.read((char*)&out, sizeof(T));
		return out;
	}

	//read functions
	int8_t readInt8();
	uint8_t readUInt8();
	int16_t readInt16();
	uint16_t readUInt16();
	int32_t readInt32();
	uint32_t readUInt32();
	float readFloat();
	double readDouble();
	char* readBytes(std::size_t);
	std::string readWString(std::size_t);
	std::string readString(std::size_t);
	void readBytesTo(uint8_t*, std::size_t);

	//write functions
	void writeInt8(int8_t);
	void writeUInt8(uint8_t);
	void writeInt16(int16_t);
	void writeUInt16(uint16_t);
	void writeInt32(int32_t);
	void writeUInt32(uint32_t);
	void writeFloat(float);
	void writeDouble(double);
	void writeBytes(uint8_t*, std::size_t);
	void writeString(std::string);

	void alignTo(std::size_t);

	void writeOffsetAt16(std::size_t);
	void writeOffsetAt32(std::size_t);

	//utility functions
	std::size_t getSize();
	std::size_t tell();
	bool seek(std::size_t, bool = false);
	void skip(std::size_t);
	std::string getPath();

	int8_t peekInt8(std::size_t);
	uint8_t peekUInt8(std::size_t);

	int16_t peekInt16(std::size_t);
	uint16_t peekUInt16(std::size_t);

	int32_t peekInt32(std::size_t);
	uint32_t peekUInt32(std::size_t);

	std::string peekString(std::size_t, std::size_t);

	std::fstream &getStream();

	CFileStream(std::string, Endianess, OpenMode mod = OpenMode::In);
	CFileStream(std::string, OpenMode mod = OpenMode::In);
	CFileStream() {}
	~CFileStream() {this->base.close();}
};

class CMemoryStream : public CStream {
	private:
		uint8_t* mBuffer;
		std::size_t mPosition;
		std::size_t mSize;
		std::size_t mCapacity;
		int8_t mHasInternalBuffer;

		OpenMode mOpenMode;
		Endianess order;
		Endianess systemOrder;

	public:
		bool Reserve(std::size_t);
		bool setSize(std::size_t);

		std::size_t getSize();
		std::size_t getCapacity();

		int8_t readInt8();
		uint8_t readUInt8();

		int16_t readInt16();
		uint16_t readUInt16();

		int32_t readInt32();
		uint32_t readUInt32();

		float readFloat();
		double readDouble();

		int8_t peekInt8(std::size_t);
		uint8_t peekUInt8(std::size_t);

		int16_t peekInt16(std::size_t);
		uint16_t peekUInt16(std::size_t);

		int32_t peekInt32(std::size_t);
		uint32_t peekUInt32(std::size_t);

		void writeInt8(int8_t);
		void writeUInt8(uint8_t);

		void writeInt16(int16_t);
		void writeUInt16(uint16_t);

		void writeInt32(int32_t);
		void writeUInt32(uint32_t);

		void writeDouble(double);
		void writeFloat(float);
		void writeBytes(uint8_t*, std::size_t);
		void writeString(std::string);

		void alignTo(std::size_t);

		void writeOffsetAt16(std::size_t);
		void writeOffsetAt32(std::size_t);

		std::string readString(std::size_t);
		std::string peekString(std::size_t, std::size_t);
		void readBytesTo(uint8_t*, std::size_t);

		bool seek(std::size_t, bool = false);
		void skip(std::size_t);
		std::size_t tell();

		uint8_t* getBuffer();

		bool changeMode(OpenMode mode);

		CMemoryStream(uint8_t*, std::size_t, Endianess, OpenMode);
		CMemoryStream(std::size_t, Endianess, OpenMode);
		CMemoryStream(){}
		~CMemoryStream(){
			if(mHasInternalBuffer){
				delete[] mBuffer;
			}
		}

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

CFileStream::CFileStream(std::string path, Endianess ord, OpenMode mod){
	base.open(path, (mod == OpenMode::In ? std::ios::in : std::ios::out) | std::ios::binary);
	base.exceptions(std::ifstream::badbit);
	filePath = path;
	order = ord;
	mode = mod;
	systemOrder = getSystemEndianess();
}

CFileStream::CFileStream(std::string path, OpenMode mod){
	base.open(path, (mod == OpenMode::In ? std::ios::in : std::ios::out) | std::ios::binary);
	base.exceptions(std::ifstream::badbit);
	filePath = path;
	mode = mod;
	systemOrder = getSystemEndianess();
	order = getSystemEndianess();
}

std::fstream &CFileStream::getStream(){
	return base;
}

std::string CFileStream::getPath(){
	return filePath;
}

bool CFileStream::seek(std::size_t pos, bool fromCurrent){
	try {
		base.seekg(pos, (fromCurrent ? base.cur : base.beg));
		return true;
	} catch (const std::ifstream::failure& f) {
		return false;
	}
}

void CFileStream::skip(std::size_t amount){
	base.seekg(amount, base.cur);
}

std::size_t CFileStream::tell(){
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

double CFileStream::readDouble(){
	assert(mode == OpenMode::In);
	char buff[sizeof(double)];
	base.read(buff, sizeof(double));
	if(order != systemOrder){
		char temp[sizeof(double)];
		temp[0] = buff[7];
		temp[1] = buff[6];
		temp[2] = buff[5];
		temp[3] = buff[4];
		temp[4] = buff[3];
		temp[5] = buff[2];
		temp[6] = buff[1];
		temp[7] = buff[0];
		return *((double*)temp);
	}
	return *((double*)buff);
}

char* CFileStream::readBytes(std::size_t size){
	assert(mode == OpenMode::In);
	char* buffer = new char[size];
	base.read(buffer, size);
	return buffer;
}

void CFileStream::readBytesTo(uint8_t* out_buffer, std::size_t len){
	assert(mode == OpenMode::In);
	base.read((char*)out_buffer, len);
}

std::string CFileStream::readString(std::size_t len){
	assert(mode == OpenMode::In);
    std::string str(len, '\0'); //creates string str at size of length and fills it with '\0'
    base.read(&str[0], len);
    return str;
}

std::string CFileStream::peekString(std::size_t at, std::size_t len){
	assert(mode == OpenMode::In);
    std::string str(len, '\0'); //creates string str at size of length and fills it with '\0'
	std::size_t cur = base.tellg();
	base.seekg(at, base.beg);
    base.read(&str[0], len);
	base.seekg(cur, base.beg);
    return str;
}

std::string CFileStream::readWString(std::size_t len){
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

void CFileStream::writeDouble(double v){
	assert(mode == OpenMode::Out);
	char* buff = (char*)&v;
	if(order != systemOrder){
		char temp[sizeof(double)];
		temp[0] = buff[7];
		temp[1] = buff[6];
		temp[2] = buff[5];
		temp[3] = buff[4];
		temp[4] = buff[3];
		temp[5] = buff[2];
		temp[6] = buff[1];
		temp[7] = buff[0];
		v = *((double*)temp);
	}
	base.write((char*)&v, sizeof(double));
}


void CFileStream::alignTo(std::size_t to){
    std::size_t nextAligned = (-base.tellg() % to) % to;
    for(int i = 0; i < nextAligned; i++) writeUInt8(0);
}

void CFileStream::writeOffsetAt16(std::size_t at){
	assert(mode == OpenMode::Out);
	uint32_t offset = base.tellg();
	uint16_t writeOffset = offset & 0xFFFF;
	if(order != systemOrder){
		writeOffset = swap16(writeOffset);
	}
	base.seekg(at);
	base.write((char*)&writeOffset, sizeof(uint32_t));
	base.seekg(offset);
}

void CFileStream::writeOffsetAt32(std::size_t at){
	assert(mode == OpenMode::Out);
	uint32_t offset = base.tellg();
	uint32_t writeOffset = offset;
	if(order != systemOrder){
		writeOffset = swap32(writeOffset);
	}
	base.seekg(at);
	base.write((char*)&writeOffset, sizeof(uint32_t));
	base.seekg(offset);
}

void CFileStream::writeString(std::string v){
	assert(mode == OpenMode::Out);
	base.write(v.c_str(), v.size());
}

void CFileStream::writeBytes(uint8_t* v, std::size_t size){
	assert(mode == OpenMode::Out);
	base.write((char*)v, size);
}

uint8_t CFileStream::peekUInt8(std::size_t offset){
	assert(mode == OpenMode::In);
	uint8_t ret;
	int pos = (int)base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt8();
	base.seekg(pos, base.beg);
	return ret;
}

int8_t CFileStream::peekInt8(std::size_t offset){
	assert(mode == OpenMode::In);
	int8_t ret;
	int pos = (int)base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt8();
	base.seekg(pos, base.beg);
	return ret;
}

uint16_t CFileStream::peekUInt16(std::size_t offset){
	assert(mode == OpenMode::In);
	uint16_t ret;
	int pos = (int)base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt16();
	base.seekg(pos, base.beg);
	return ret;
}

int16_t CFileStream::peekInt16(std::size_t offset){
	assert(mode == OpenMode::In);
	int16_t ret;
	int pos = (int)base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt16();
	base.seekg(pos, base.beg);
	return ret;
}

uint32_t CFileStream::peekUInt32(std::size_t offset){
	assert(mode == OpenMode::In);
	uint32_t ret;
	int pos = (int)base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt32();
	base.seekg(pos, base.beg);
	return ret;
}

int32_t CFileStream::peekInt32(std::size_t offset){
	assert(mode == OpenMode::In);
	int32_t ret;
	int pos = (int)base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt32();
	base.seekg(pos, base.beg);
	return ret;
}

std::size_t CFileStream::getSize(){
	int pos = (int)base.tellg();
	base.seekg(0, std::ios::end);
	std::size_t ret = base.tellg();
	base.seekg(pos, std::ios::beg);
	return ret;
}

///
///
///  CMemoryStream
///
///


CMemoryStream::CMemoryStream(uint8_t* ptr, std::size_t size, Endianess ord, OpenMode mode){
	mBuffer = ptr;
	mPosition = 0;
	mSize = size;
	mCapacity = size;
	mHasInternalBuffer = false;
	mOpenMode = mode;
	order = ord;
	systemOrder = getSystemEndianess();
}

CMemoryStream::CMemoryStream(std::size_t size, Endianess ord, OpenMode mode){
	mBuffer = new uint8_t[size]{};
	mPosition = 0;
	mSize = size;
	mCapacity = size;
	mHasInternalBuffer = true;
	mOpenMode = mode;
	order = ord;
	systemOrder = getSystemEndianess();
}

std::size_t CMemoryStream::getSize(){
	return mSize;
}

std::size_t CMemoryStream::getCapacity(){
	return mCapacity;
}

// Allow for changing from read to write mode ONLY if we have an internal buffer.
bool CMemoryStream::changeMode(OpenMode mode){
	if(!mHasInternalBuffer){
		return false;
	}

	mOpenMode = mode;

	return true;
}

bool CMemoryStream::seek(std::size_t pos, bool fromCurrent){
	if(fromCurrent && mPosition + pos > mCapacity || pos > mCapacity) return false;

	if(fromCurrent){
		mPosition += pos;
	} else {
		mPosition = pos;
	}

	if(mPosition > mSize) mSize = mPosition;

	return true;
}

void CMemoryStream::skip(std::size_t amount){
	mPosition += (mPosition + amount < mSize ? amount : 0);
}

std::size_t CMemoryStream::tell(){
	return mPosition;
}

uint8_t* CMemoryStream::getBuffer(){
	return mBuffer;
}

///
/// Memstream Reading Functions
///

int8_t CMemoryStream::readInt8(){
	assert(mOpenMode == OpenMode::In && mPosition < mSize);
	int8_t r;
	memcpy(&r, OffsetPointer<int8_t>(mBuffer, mPosition), sizeof(int8_t));
	mPosition++;
	return r;
}

uint8_t CMemoryStream::readUInt8(){
	assert(mOpenMode == OpenMode::In);
	uint8_t r;
	memcpy(&r, OffsetPointer<uint8_t>(mBuffer, mPosition), sizeof(uint8_t));
	mPosition++;
	return r;
}

int16_t CMemoryStream::readInt16(){
	assert(mOpenMode == OpenMode::In && mPosition < mSize);
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
	assert(mOpenMode == OpenMode::In && mPosition < mSize);
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
	assert(mOpenMode == OpenMode::In && mPosition < mSize);
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
	assert(mOpenMode == OpenMode::In && mPosition < mSize);
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


float CMemoryStream::readFloat(){
	assert(mOpenMode == OpenMode::In && mPosition < mSize);

	char buff[sizeof(float)];
	memcpy(&buff, OffsetPointer<int32_t>(mBuffer, mPosition), sizeof(float));
	mPosition += sizeof(float);
	if(order != systemOrder){
		char temp[sizeof(float)];
		temp[0] = buff[3];
		temp[1] = buff[2];
		temp[2] = buff[1];
		temp[3] = buff[0];
		return *((float*)temp);
	}

	else{
		return *((float*)buff);
	}
}

double CMemoryStream::readDouble(){
	assert(mOpenMode == OpenMode::In && mPosition < mSize);

	char buff[sizeof(double)];
	memcpy(&buff, OffsetPointer<double>(mBuffer, mPosition), sizeof(float));
	mPosition += sizeof(double);
	if(order != systemOrder){
		char temp[sizeof(double)];
		temp[0] = buff[3];
		temp[1] = buff[2];
		temp[2] = buff[1];
		temp[3] = buff[0];
		return *((double*)temp);
	}

	else{
		return *((double*)buff);
	}
}

///
/// Memstream Peek Functions
///

int8_t CMemoryStream::peekInt8(std::size_t at){
	assert(mOpenMode == OpenMode::In && at < mSize);
	int8_t r;
	memcpy(&r, OffsetPointer<int8_t>(mBuffer, at), sizeof(int8_t));
	return r;
}

uint8_t CMemoryStream::peekUInt8(std::size_t at){
	assert(mOpenMode == OpenMode::In && at < mSize);
	uint8_t r;
	memcpy(&r, OffsetPointer<uint8_t>(mBuffer, at), sizeof(uint8_t));
	return r;
}

int16_t CMemoryStream::peekInt16(std::size_t at){
	assert(mOpenMode == OpenMode::In && at < mSize);
	int16_t r;
	memcpy(&r, OffsetPointer<int16_t>(mBuffer, at), sizeof(int16_t));

	if(order != systemOrder){
		return swap16(r);
	}
	else{
		return r;
	}
}

uint16_t CMemoryStream::peekUInt16(std::size_t at){
	assert(mOpenMode == OpenMode::In && at < mSize);
	uint16_t r;
	memcpy(&r, OffsetPointer<uint16_t>(mBuffer, at), sizeof(uint16_t));

	if(order != systemOrder){
		return swap16(r);
	}
	else{
		return r;
	}
}

uint32_t CMemoryStream::peekUInt32(std::size_t at){
	assert(mOpenMode == OpenMode::In && at < mSize);
	uint32_t r;
	memcpy(&r, OffsetPointer<uint32_t>(mBuffer, at), sizeof(uint32_t));

	if(order != systemOrder){
		return swap32(r);
	}
	else{
		return r;
	}
}

int32_t CMemoryStream::peekInt32(std::size_t at){
	assert(mOpenMode == OpenMode::In && at < mSize);
	int32_t r;
	memcpy(&r, OffsetPointer<int32_t>(mBuffer, at), sizeof(int32_t));

	if(order != systemOrder){
		return swap32(r);
	}
	else{
		return r;
	}
}

std::string CMemoryStream::readString(std::size_t len){
	assert(mOpenMode == OpenMode::In && mPosition < mSize);
	std::string str(OffsetPointer<char>(mBuffer, mPosition),OffsetPointer<char>(mBuffer, mPosition+len));
	mPosition += len;
	return str;
}

std::string CMemoryStream::peekString(std::size_t at, std::size_t len){
	assert(mOpenMode == OpenMode::In && mPosition < mSize);
	std::string str(OffsetPointer<char>(mBuffer, at), OffsetPointer<char>(mBuffer, at+len));
	return str;
}

//I don't like this set up, but for now it works
void CMemoryStream::readBytesTo(uint8_t* out_buffer, std::size_t len){
	assert(mOpenMode == OpenMode::In && mPosition < mSize);
	if(mPosition + len <= mSize){
		memcpy(out_buffer, OffsetPointer<char>(mBuffer, mPosition), len);

		mPosition += len;
	}
}

///
/// Memstream Writing Functions
///

bool CMemoryStream::setSize(std::size_t size) {
	if(mCapacity >= size){
		mSize = size;
		return true;
	}
	if(!mHasInternalBuffer){
		return false;
	}

	mCapacity = size;
	uint8_t* temp = new uint8_t[mCapacity]{};
	memcpy(temp, mBuffer, mSize);
	delete[] mBuffer;
	mBuffer = temp;
	mSize = size;

	return true;
}

//included in writing functions because this is needed when using an internal buffer
bool CMemoryStream::Reserve(std::size_t needed){
	if(mCapacity >= needed){
		return true;
	}
	if(!mHasInternalBuffer){
		return false;
	}

	if(mCapacity * 2 > needed){
		mCapacity *= 2;
	} else {
		mCapacity += needed;
	}

	uint8_t* temp = new uint8_t[mCapacity]{};
	memcpy(temp, mBuffer, mSize);
	delete[] mBuffer;
	mBuffer = temp;

	mSize += needed;
	return true;
}

void CMemoryStream::writeInt8(int8_t v){
	Reserve(mPosition + sizeof(v));
	memcpy(OffsetWritePointer<int8_t>(mBuffer, mPosition), &v, sizeof(int8_t));
	mPosition += sizeof(int8_t);
}

void CMemoryStream::writeUInt8(uint8_t v){
	Reserve(mPosition + sizeof(v));
	memcpy(OffsetWritePointer<uint8_t>(mBuffer, mPosition), &v, sizeof(int8_t));
	mPosition += sizeof(int8_t);
}

void CMemoryStream::writeInt16(int16_t v){
	Reserve(mPosition + sizeof(v));

	if (order != systemOrder)
		v = swap16(v);

	memcpy(OffsetWritePointer<int16_t>(mBuffer, mPosition), &v, sizeof(int16_t));
	mPosition += sizeof(int16_t);
}

void CMemoryStream::writeUInt16(uint16_t v){
	Reserve(mPosition + sizeof(v));

	if (order != systemOrder)
		v = swap16(v);

	memcpy(OffsetWritePointer<uint16_t>(mBuffer, mPosition), &v, sizeof(int16_t));
	mPosition += sizeof(int16_t);
}

void CMemoryStream::writeInt32(int32_t v){
	Reserve(mPosition + sizeof(v));

	if (order != systemOrder)
		v = swap32(v);

	memcpy(OffsetWritePointer<int32_t>(mBuffer, mPosition), &v, sizeof(int32_t));
	mPosition += sizeof(int32_t);
}

void CMemoryStream::writeUInt32(uint32_t v){
	Reserve(mPosition + sizeof(v));

	if (order != systemOrder)
		v = swap32(v);

	memcpy(OffsetWritePointer<uint32_t>(mBuffer, mPosition), &v, sizeof(int32_t));
	mPosition += sizeof(int32_t);
}

void CMemoryStream::writeFloat(float v){
	Reserve(mPosition + sizeof(v));

	char* buff = (char*)&v;
	if(order != systemOrder){
		char temp[sizeof(float)];
		temp[0] = buff[3];
		temp[1] = buff[2];
		temp[2] = buff[1];
		temp[3] = buff[0];
		v = *((float*)temp);
	}

	memcpy(OffsetWritePointer<float>(mBuffer, mPosition), &v, sizeof(float));
	mPosition += sizeof(float);
}


void CMemoryStream::writeDouble(double v){
	Reserve(mPosition + sizeof(v));

	char* buff = (char*)&v;
	if(order != systemOrder){
		char temp[sizeof(float)];
		temp[0] = buff[7];
		temp[1] = buff[6];
		temp[2] = buff[5];
		temp[3] = buff[4];
		temp[4] = buff[3];
		temp[5] = buff[2];
		temp[6] = buff[1];
		temp[7] = buff[0];
		v = *((double*)temp);
	}

	memcpy(OffsetWritePointer<double>(mBuffer, mPosition), &v, sizeof(double));
	mPosition += sizeof(double);
}

//TODO: Clean these up and test them more

void CMemoryStream::writeBytes(uint8_t* bytes, std::size_t size){
	Reserve(mPosition + size);
	memcpy(OffsetWritePointer<uint8_t>(mBuffer, mPosition), bytes, size);
	mPosition += size;
}

void CMemoryStream::writeString(std::string str){
	Reserve(mPosition + str.size());
	memcpy(OffsetWritePointer<uint8_t>(mBuffer, mPosition), str.data(), str.size());
	mPosition += str.size();
}

void CMemoryStream::alignTo(std::size_t to){
    std::size_t nextAligned = (-mPosition % to) % to;
    Reserve(mPosition+nextAligned);
    memset(OffsetWritePointer<uint8_t>(mBuffer, mPosition), 0, nextAligned);
    mPosition += nextAligned;
}

void CMemoryStream::writeOffsetAt16(std::size_t at){
	Reserve(mPosition + sizeof(uint16_t));
	uint16_t offset = mPosition & 0xFFFF;
	if(order != systemOrder){
		offset = swap16(offset);
	}
	memcpy(OffsetWritePointer<uint16_t>(mBuffer, at), &offset, sizeof(uint16_t));
}

void CMemoryStream::writeOffsetAt32(std::size_t at){
	Reserve(mPosition + sizeof(uint32_t));
	uint32_t offset = mPosition;
	if(order != systemOrder){
		offset = swap32(offset);
	}
	memcpy(OffsetWritePointer<uint32_t>(mBuffer, at), &offset, sizeof(uint32_t));
}

}
#endif

#endif
