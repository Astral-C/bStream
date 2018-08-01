#include "bstream.h"

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
CFileStream::CFileStream(std::string path, Endianess ord, OpenMode mode){
	base.open(path, (mode == OpenMode::In ? std::ios::in : std::ios::out) | std::ios::binary);
	filePath = path;
	order = ord;
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
	uint8_t r;
	base.read((char*)&r, sizeof(uint8_t));
	return r;
}

int8_t CFileStream::readInt8(){
	int8_t r;
	base.read((char*)&r, sizeof(int8_t));
	return r;
}

//todo: think of how to clean this up
float CFileStream::readFloat(){
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
	char* buffer = new char[size];
	base.read(buffer, size);
	return buffer;
}

std::string CFileStream::readString(size_t len){
    std::string str(len, '\0'); //creates string str at size of length and fills it with '\0'
    base.read(&str[0], len);
    return str;
}

std::string CFileStream::readWString(size_t len){
    std::string str(len, '\0'); //creates string str at size of length and fills it with '\0'
    base.read(&str[0], len);
    return str;
}

void CFileStream::writeInt8(int8_t v){
	base.write((char*)&v, 1);
}

void CFileStream::writeUInt8(uint8_t v){
	base.write((char*)&v, 1);
}

void CFileStream::writeInt16(int16_t v){
	if(order != systemOrder){
		v = swap16(v);
	}
	base.write((char*)&v, sizeof(uint16_t));
}

void CFileStream::writeUInt16(uint16_t v){
	if(order != systemOrder){
		v = swap16(v);
	}
	base.write((char*)&v, sizeof(uint16_t));
}

void CFileStream::writeInt32(int32_t v){
	if(order != systemOrder){
	   v = swap32(v);
	}
	base.write((char*)&v, sizeof(int32_t));
}

void CFileStream::writeUInt32(uint32_t v){
	if(order != systemOrder){
	   v = swap32(v);
	}
	base.write((char*)&v, sizeof(uint32_t));
}

void CFileStream::writeFloat(float v){
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
	base.write(v.c_str(), v.size());
}

void CFileStream::writeBytes(char* v, size_t size){
	base.write(v, size);
}

void CFileStream::readStruct(void* out, size_t size){
	base.read((char*)out, size);
}

uint8_t CFileStream::peekU8(int offset){
	uint8_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt8();
	base.seekg(pos, base.beg);
	return ret;
}

int8_t CFileStream::peekI8(int offset){
	int8_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt8();
	base.seekg(pos, base.beg);
	return ret;
}

uint16_t CFileStream::peekU16(int offset){
	uint16_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt16();
	base.seekg(pos, base.beg);
	return ret;
}

int16_t CFileStream::peekI16(int offset){
	int16_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt16();
	base.seekg(pos, base.beg);
	return ret;
}

uint32_t CFileStream::peekU32(int offset){
	uint32_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt32();
	base.seekg(pos, base.beg);
	return ret;
}

int32_t CFileStream::peekI32(int offset){
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