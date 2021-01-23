#include "types.h"
#include <string>

inline bool charCmp(char a, char b) {
	return(toupper(a) == toupper(b));
}

bool strEqu(std::string s1, std::string s2) {
	return((s1.size() == s2.size()) &&
			std::equal(s1.begin(), s1.end(), s2.begin(), charCmp));
}

bool setContains(const std::string arr[], std::string str) {
	int i = 0;
	while (arr[i] != "") {
		if (strEqu(arr[i], str))
			return true;
		i++;
	}
	return false;
}

bool isWhitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}

bool isOperand(char c) {
	return operands.find(c,0) != operands.npos;
}

bool isPunct(char c) {
	return punctuations.find(c,0) != punctuations.npos;
}


bool isRegister(std::string str) {
	return setContains(registers, str);
}

bool isJmpInstr(std::string str) {
	return setContains(jumps, str);
}

bool isKeyword(std::string str) {
	return setContains(keywords, str);
}

bool isConversionKword(std::string str) {
	return setContains(conversionKwords, str);
}

bool isLoaded(Type t) {
	return t == NUM || t == REG;
}

bool isAdditive(std::string str) {
	return str == "+" || str == "-";
}

std::string toRealRegister(std::string reg) {
	int i = 0;
	while (registers[i] != "") {
		if (registers[i] == reg)
			return registers32[i];
		i++;
	}
	return "";
}

std::string toRealRegister(std::string reg, int size) {
	int i = 0;
	while (registers[i] != "") {
		if (registers[i] == reg) {
			if (size == 8) return registers64[i];
			if (size == 4) return registers32[i];
			if (size == 2) return registers16[i];
			if (size == 1) return registers8[i];
		}
		i++;
	}
	return "";
}

bool keywordNeedsArgument(std::string kw) {
	return strEqu(kw, "CALL")
		|| strEqu(kw, "SECTION")
		|| strEqu(kw, "GLOBAL")
		|| strEqu(kw, "EXTERN");
}