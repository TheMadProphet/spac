#include "compiler.h"
#include "types.h"
#include <string>

Compiler::Compiler() {
	outFile = std::ofstream("a.asm");


	// Generate 'headers'
}

Compiler::~Compiler() {
	
}

void Compiler::Compile(Parser *parser) {
	generateDataSection();
	generateBssSection();
	generateTextSection(parser);
}

void Compiler::generateDataSection() {
	writeLine("SECTION .data			; Section containing initialized data");

	writeLine("");
}

void Compiler::generateBssSection() {
	writeLine("SECTION .bss			; Section containing uninitialized data");

	writeInstr("savedEAX resb 4");
	writeInstr("savedEDX resb 4");

	writeLine("");
}

void Compiler::generateTextSection(Parser *parser) {
	writeLine("SECTION .text			; Section containing code\n");
	writeLine("global main\n");
	writeLine("main:");
	writeInstr("nop");

	std::vector<StatementStruct *> statements = parser->parse();

	for (int i = 0; i < statements.size(); i++) {
		translate(statements.at(i));
	}
}

void Compiler::translate(StatementStruct *statement) {
	writeInstr("; Line " + std::to_string(statement->line));
	switch (statement->type) {
	case LOAD:
		translateLoad(statement->ptr);
		break;

	case STORE:
		translateStore(statement->ptr);
		break;

	case JUMP:
		translateJump(statement->ptr);
		break;

	case CONVERT:
		translateConvert(statement->ptr);
		break;

	case KEYWORD:
		translateKeyword(statement->ptr);
		break;

	default:
		break;
	}
	writeLine("");
}

void Compiler::translateStore(void *ptr) {
	Parser::StoreStruct *structure = (Parser::StoreStruct *) ptr;
	
	if (structure->dest.operation != "") {
		translateExpression(structure->dest);
		std::string line = "mov "; 
		line += operationSpecifier(structure->store) + " [EAX], ";
		line += instantToString(structure->val, structure->store);
		writeInstr(line);
		writeInstr("mov EAX, [savedEAX]");
	} else {
		std::string line = "mov " + operationSpecifier(structure->store);
		line += " [" + instantToString(structure->dest.operand1) + "], ";
		line += instantToString(structure->val, structure->store);
		writeInstr(line);
	}
}

void Compiler::translateLoad(void *ptr) {
	Parser::LoadStruct *structure = (Parser::LoadStruct *) ptr;

	if (structure->expr.operation != "") {
		translateExpression(structure->expr);
		if (structure->reference)
			writeInstr("mov " + toRealRegister(structure->reg, structure->load) +", [EAX]");
		else
			writeInstr("mov " + toRealRegister(structure->reg, structure->load) +", EAX");
		writeInstr("mov EAX, [savedEAX]");
	} else {
		writeInstr("mov " + toRealRegister(structure->reg, structure->load) + ", "
					+ instantToString(structure->expr.operand1, structure->load));
	}
}

void Compiler::translateJump(void *ptr) {

}

void Compiler::translateConvert(void *ptr) {

}

void Compiler::translateKeyword(void *ptr) {

}

std::string Compiler::operationSpecifier(int size) {
	switch (size) {
	case 1:
		return "byte";
	
	case 2:
		return "word";

	case 4:
		return "dword";

	default:
		return "dword.";
	}
}

std::string Compiler::instantToString(Token tok) {
	if (tok.type == REG)
		return toRealRegister(tok.value);
	
	return tok.value;
}

std::string Compiler::instantToString(Token tok, int size) {
	if (tok.type == REG)
		return toRealRegister(tok.value, size);
	
	return tok.value;
}

void Compiler::writeOut(std::string str) {
	outFile.write(str.c_str(), str.length());
}

void Compiler::writeLine(std::string line) {
	writeOut(line + "\n");
}

void Compiler::writeInstr(std::string instr) {
	writeLine("\t" + instr);
}

void Compiler::translateExpression(ExprStruct expr) {
	writeInstr("mov dword [savedEAX], EAX");
	writeInstr("mov EAX, " + instantToString(expr.operand1));
	if (expr.operation == "") return;

	switch (expr.operation[0]) {
	case '+':
		writeInstr("add EAX, " + instantToString(expr.operand2));
		break;

	case '-':
		writeInstr("sub EAX, " + instantToString(expr.operand2));
		break;

	case '/':
		translateExpressionDiv(expr);
		break;

	case '*':
		writeInstr("imul EAX, " + instantToString(expr.operand2));
		break;

	case '^':
		writeInstr("xor EAX, " + instantToString(expr.operand2));
		break;

	case '&':
		writeInstr("and EAX, " + instantToString(expr.operand2));
		break;

	case '|':
		writeInstr("or EAX, " + instantToString(expr.operand2));
		break;

	default:
		break;
	}
}

void Compiler::translateExpressionDiv(ExprStruct expr) {
	writeInstr("mov dword [savedEDX], EDX");
	writeInstr("mov EDX, 0");
	writeInstr("idiv " + instantToString(expr.operand2));
	writeInstr("mov EDX, [savedEDX]");
}