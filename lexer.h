#ifndef _LEXER_H_
#define _LEXER_H_

#include <iostream>
#include "inputStream.h"

enum Type { PUNC,	// Punctiation
 			NUM,	// Number
			STR,	// String
			CHR,	// Character
			KW,		// Keyword
			JMP,	// Jump
			REG,	// Register
			OP,		// Operation
			REF,	// Reference
			CONV,	// Convert
			END,	// End (Assigned when reaching end-of-file)
			ERR		// Unassigned type
			};

struct Token {
	Type type;
	std::string value;
	int col;
	int row;
	
	Token() {
		value = "";
	}

	Token(Type t) {
		type = t;
		value = "";
	}

	Token(Type t, std::string str) {
		type = t;
		value = str;
	}

	Token(Type t, std::string str, int _col, int _row) {
		type = t;
		value = str;
		col = _col;
		row = _row;
	}

	Token(Type t, inputToken inpTok) {
		type = t;
		value = inpTok.c;
		col = inpTok.col;
		row = inpTok.row;
	}
};


class Lexer {
private:
	InputStream *is;
	Token current;
	Token next;

	Token _read();
	
	void skipComment();

	Token readString();
	Token readCharacter();
	Token readNumber();
	Token readOperand();
	Token readIdentifier();
public:
	Lexer(InputStream *inpStream);
	~Lexer();

	Token read();
	Token peek();
	Token peekExtra();
};

#endif