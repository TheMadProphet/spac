#include "inputStream.h"
#include <assert.h>
#include <fstream>

InputStream::InputStream(std::string fileName) {
	openFile(fileName);
	col = 1;
	row = 1;
}

InputStream::~InputStream() {
}

inputToken InputStream::peek() {
	assert(fd.is_open());

	inputToken res(fd.peek(), col, row);
	return res;
}

inputToken InputStream::peek(int n) {
	assert(fd.is_open());
	fd.seekg(n, std::ios::cur);
	inputToken res(fd.peek(), col+n, row);
	fd.seekg(-n, std::ios::cur);
	return res;
}

inputToken InputStream::consume() {
	assert(fd.is_open());

	inputToken res = peek();
	fd.get();
	if (res.c == '\n') {
		col = 0;
		row++;
	}
	else
		col++;
	return res;
}

bool InputStream::eof() {
	assert(fd.is_open());
	return fd.eof();
}

void InputStream::croack(std::string msg) {
	assert(fd.is_open());
	
}

bool InputStream::openFile(std::string fileName) {
	fd = std::ifstream(fileName);
	return fd.is_open();
}