#include "bstream.h"
#define OffsetPointer(ptr, offset) ((void*)((char *)(ptr) + (offset)))
namespace bStream {

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
	base.open(path, (mode == OpenMode::in ? std::ios::in : std::ios::out));
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
		return ( ((r>>24)&0xFF) | ((r<<8) & 0xFF0000) | ((r>>8)&0xFF00) | ((r<<24)&0xFF000000));
	}
	else{
		return r;
	}
}

int32_t CFileStream::readInt32(){
	int32_t r;
	base.read((char*)&r, sizeof(int32_t));
	if(order != systemOrder){
		return ( ((r>>24)&0xFF) | ((r<<8) & 0xFF0000) | ((r>>8)&0xFF00) | ((r<<24)&0xFF000000));
	}
	else{
		return r;
	}
}

uint16_t CFileStream::readUInt16(){
	uint16_t r;
	base.read((char*)&r, sizeof(uint16_t));
	if(order != systemOrder){
		return ( ((r<<8)&0xFF00) | ((r>>8)&0x00FF) );
	}
	else{
		return r;
	}
}

int16_t CFileStream::readInt16(){
	int16_t r;
	base.read((char*)&r, sizeof(int16_t));
	if(order != systemOrder){
		return ( ((r<<8)&0xFF00) | ((r>>8)&0x00FF) );
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

char* CFileStream::readBytes(int count){
	char* buffer = new char[count];
	base.read(buffer, (size_t)count);
	return buffer;
}

std::string CFileStream::readString(int len){
	char* str = (char*)std::malloc(len);
	base.read(str, len);
	std::string stdstr(str);
	std::free(str);
	return stdstr;
}

void CFileStream::writeInt8(int8_t v){
	base.write((char*)&v, 1);
}

void CFileStream::writeUInt8(uint8_t v){
	base.write((char*)&v, 1);
}

void CFileStream::writeInt16(int16_t v){
	if(order != systemOrder){
		v = (((v<<8)&0xFF00) | ((v>>8)&0x00FF));
	}
	base.write((char*)&v, sizeof(uint16_t));
}

void CFileStream::writeUInt16(uint16_t v){
	if(order != systemOrder){
		v = (((v<<8)&0xFF00) | ((v>>8)&0x00FF));
	}
	base.write((char*)&v, sizeof(uint16_t));
}

void CFileStream::writeInt32(int32_t v){
	if(order != systemOrder){
	   v = (((v>>24)&0xFF) | ((v<<8) & 0xFF0000) | ((v>>8)&0xFF00) | ((v<<24)&0xFF000000));
	}
	base.write((char*)&v, sizeof(int32_t));
}

void CFileStream::writeUInt32(uint32_t v){
	if(order != systemOrder){
	   v = (((v>>24)&0xFF) | ((v<<8) & 0xFF0000) | ((v>>8)&0xFF00) | ((v<<24)&0xFF000000));
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

void CMemoryStream::seek(size_t pos){
	mPosition = (pos > mSize ? mPosition : pos);
}

//TODO: Make it so that you cant read past the end of the stream

uint32_t CMemoryStream::readUInt32(){
	
	uint32_t r;
	memcpy(&r, OffsetPointer(mBuffer, mPosition), sizeof(uint32_t));
	mPosition += sizeof(uint32_t);

	if(order != systemOrder){
		return ( ((r>>24)&0xFF) | ((r<<8) & 0xFF0000) | ((r>>8)&0xFF00) | ((r<<24)&0xFF000000));
	}
	else{
		return r;
	}
}

std::string CMemoryStream::readString(int len){
	char str[len];
	strncpy(str, (char*)OffsetPointer(mBuffer, mPosition), len);
	mPosition += len;
	return std::string(str);
}

}