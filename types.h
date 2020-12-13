#ifndef _TYPES_H_
#define _TYPES_H_

#include <iostream>
#include <unordered_set>
#include "lexer.h"

static const std::string punctuations = ",;(){}[]<>";
static const std::string operands = "+-*/|&^=";

static const std::string registers[] =   {"R1" , "R2" , "R3" , "R4" , "R5" , "R6" , "R7" , "R8" , "SP" , "PC" , "RV" , ""};
static const std::string registers64[] = {"RDI", "RSI", "RDX", "RCX", "R8" , "R9" , "R10", "R11", "RSP", "RIP", "RAX", ""};
static const std::string registers32[] = {"EDI", "ESI", "EDX", "ECX", "R8D", "R9D", "R10D","R11D","ESP", "RIP", "EAX", ""};
static const std::string registers16[] = {"DI" , "SI" , "DX" , "CX" , "R8W", "R9W", "R10W","R11W","SP" , "RIP", "AX" , ""};
static const std::string registers8[] =  {"DIL", "SIL", "DL" , "CL" , "R8B", "R9B", "R10B","R11B","SPL", "RIP", "AL" , ""};
static const std::string jumps[] = {"BLT","BLE","BGT","BGE","BEQ","BNE","Jmp", ""};
static const std::string keywords[] = {"CALL", "RET", "SYSCALL", "SECTION", "", ".text", ""};
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