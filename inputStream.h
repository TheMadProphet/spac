#ifndef _INPUT_STREAM_H_
#define _INPUT_STREAM_H_

#include <iostream>
#include <fstream>
#include <string>

struct inputToken {
	char c;
	int col;
	int row;

	inputToken() {
		c = '\0';
		col = -1;
		row = -1;
	}

	inputToken(char _c) {
		c = _c;
		col = -1;
		row = -1;
	}

	inputToken(char _c, int _col, int _row) {
		c = _c;
		col = _col;
		row = _row;
	}
};


class InputStream {
private:
	std::ifstream fd;
	int col;
	int row;
	bool openFile(std::string fileName);
public:
	InputStream(std::string fileName);
	~InputStream();

	inputToken peek();
	inputToken peek(int n);
	inputToken consume();
	bool eof();
	void croack(std::string msg);//?
};


#endif