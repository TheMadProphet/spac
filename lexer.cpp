#include "lexer.h"
#include <string>
#include "types.h"

Lexer::Lexer(InputStream *inpStream) {
	is = inpStream;
	current = _read();
	next = _read();
}

Lexer::~Lexer() {
}

Token Lexer::peek() {
	return current;
}

Token Lexer::peekExtra() {
	return next;
}

Token Lexer::read() {
	Token res = current;
	current = next;
	if (current.type != END)
		next = _read();
	return res;
}

Token Lexer::_read() {
	// Skip whitespace
	while(isWhitespace(is->peek().c)) {
		is->consume();
	}
	// Return end token if we reach EOF
	if (is->eof())
		return Token(END);

	char c = is->peek().c;
	
	// Skip comments
	if (c == '/' && is->peek(1).c == '/') {
		skipComment();
		return _read();
	}

	// Type cases
	if (c == '"') return readString();
	if (isdigit(c) != 0) return readNumber();
	if (isPunct(c)) return Token(PUNC, is->consume());
	if (isOperand(c)) return readOperand();
	if (isalpha(c) != 0) return readIdentifier();
	return Token(ERR, is->consume());
	is->croack("No matching type for character: " + c);
}

void Lexer::skipComment() {
	while (is->peek().c != '\n') {
		if (is->eof()) return;
		is->consume();
	}
	if (!is->eof())
		is->consume();
}

Token Lexer::readString() {
	Token res(STR, is->consume());
	std::string value = "";
	while (is->peek().c != '"') {
		value += is->consume().c;
	}
	is->consume();
	res.value = value;
	return res;
}

Token Lexer::readNumber() {
	Token res(NUM, is->peek());
	res.value = "";
	
	bool hasDot = false;
	char c = is->consume().c;
	res.value += c;
	while (true) {
		c = is->peek().c;
		if (c == '.') {
			if (hasDot)
				return res;
			else
				hasDot = true;
		}
		if (isdigit(c) != 0 || c == '.')
			res.value += c;
		else 
			break;
		is->consume();
	}
	return res;
}

Token Lexer::readOperand() {
	Token res(OP, is->consume());

	// Special case for assignment "=.n"
	if (res.value == "=" && is->peek().c == '.') {
		res.value += is->consume().c;
		while (isdigit(is->peek().c) != 0)
			res.value += is->consume().c;
	}
	return res;
}

Token Lexer::readIdentifier() {
	std::string ident = "";
	int col = is->peek().col;
	int row = is->peek().row;
	char c = is->peek().c;
	while (isalpha(c) != 0 || isdigit(c) != 0) {
		ident += is->consume().c;
		c = is->peek().c;
	}

	if (ident == "M") return Token(REF, ident, col, row);
	if (isConversionKword(ident))  return Token(CONV, ident, col, row);
	if (isRegister(ident))	return Token(REG, ident, col, row);
	if (isJmpInstr(ident))	return Token(JMP, ident, col, row);
	if (isKeyword (ident))	return Token(KW,  ident, col, row);

	return Token(ERR, ident, col, row);	// undefined
}