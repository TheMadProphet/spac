#include "types.h"

bool contains(const std::string arr[], std::string str) {
	int i = 0;
	while (arr[i] != "") {
		if (arr[i] == str)
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
	return contains(registers, str);
}

bool isJmpInstr(std::string str) {
	return contains(jumps, str);
}

bool isKeyword(std::string str) {
	return contains(keywords, str);
}

bool isConversionKword(std::string str) {
	return contains(conversionKwords, str);
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
			if (size == 4) return registers32[i];
			if (size == 2) return registers16[i];
			if (size == 1) return registers8[i];
		}
		i++;
	}
	return "";
}