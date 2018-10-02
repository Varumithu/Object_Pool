#include "testclass.h"
#include <cstring>
#include <algorithm>

stupidString::stupidString(const char * str) {
	this->length = std::strlen(str) + 1;
	this->str = new char[length];
	std::copy(str, str + length, this->str);
}
stupidString::stupidString(const stupidString& that) { // copy
	this->length = that.length;
	this->str = new char[length];
	std::copy(that.str, that.str + length, this->str);
}
stupidString::stupidString(stupidString&& that) { //move
	this->length = that.length;
	this->str = that.str;
	that.str = nullptr;
}
stupidString::~stupidString() {
	delete[] this->str;
}
void stupidString::set(const char* str) {
	delete[] this->str;
	this->length = std::strlen(str) + 1;
	this->str = new char[length];
	std::copy(str, str + length, this->str);
}

stupidString& stupidString::operator=(const stupidString& that) {
	delete[] this->str;
	this->length = that.length;
	this->str = new char[length];
	std::copy(str, str + length, this->str);
	return *this;
}