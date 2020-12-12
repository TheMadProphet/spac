#ifndef _TYPES_H_
#define _TYPES_H_

#include <iostream>
#include <unordered_set>
#include "lexer.h"

static const std::string punctuations = ",;(){}[]<>";
static const std::string operands = "+-*/%|&^=";

static const std::string registers[] =   {"R1", "R2", "R3", "R4", "R5", "SP", "PC", "RV", ""};
static const std::string registers32[] = {"EBX","ECX","EDX","ESI","EDI","ESP","EIP","EAX",""};
static const std::string registers16[] = {"BX", "CX", "DX", "SI", "DI", "SP", "EIP","AX",""};
static const std::string registers8[] =  {"BL", "CL", "DL", "SIL","DIL","SPL","EIP","AL",""};
static const std::string jumps[] = {"BLT","BLE","BGT","BGE","BEQ","BNE","Jmp", ""};
static const std::string keywords[] = {"CALL", "RET", "SYSCALL", ""};
static const std::string conversionKwords[] = {"ItoF", "FtoI", ""};

bool isWhitespace(char c);
bool isOperand(char c);
bool isPunct(char c);

bool isConversionKword(std::string str);
bool isAdditive(std::string str);
bool isRegister(std::string str);
bool isJmpInstr(std::string str);
bool isKeyword(std::string str);
bool isLoaded(Type t);

std::string toRealRegister(std::string reg);
std::string toRealRegister(std::string reg, int size);

#endif