#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "parser.h"

class Compiler {
public:
	Compiler();
	~Compiler();

	void Compile(Parser *parser);

private:
	//std::vector<StatementStruct *> statements;

	std::ofstream outFile;

	void generateDataSegment();
	void generateBssSegment();
	void generateTextSegment(Parser *parser);

	void translate(StatementStruct *statement);

	void translateLoad(void *ptr);
	void translateStore(void *ptr);
	void translateJump(void *ptr);
	void translateConvert(void *ptr);
	void translateKeyword(void *ptr);

	void translateLoadReference(Parser::LoadStruct *structure);
	void translateLoadInstant(Parser::LoadStruct *structure);

	void translateExpression(ExprStruct expr);
	void translateExpressionDiv(ExprStruct expr);
	void translateExpressionRemainder(ExprStruct expr);

	std::string operationSpecifier(int size);
	std::string instantToString(Token tok);
	std::string instantToString(Token tok, int size);

	void writeOut(std::string str);
	void writeLine(std::string line);
	void writeInstr(std::string instr);

};

#endif