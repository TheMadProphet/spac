#include <iostream>
#include "compiler.h"

using namespace std;

void printInputToken(inputToken t) {
	cout << "'" << t.c << "' - " << t.col << "," << t.row << endl;
}

void testInputStream(string fileName) {
	InputStream is(fileName);
	while (!is.eof()) {
		printInputToken(is.consume());
	}
}

string typeToString(Type t) {
	switch (t) 	{
	case PUNC:
		return "PUNC";
	case NUM:
		return "NUM";
	case STR:
		return "STR";
	case KW:
		return "KW";
	case JMP:
		return "JMP";
	case REG:
		return "REG";
	case OP:
		return "OP";
	case REF:
		return "REF";
	case CONV:
		return "CONV";
	case END:
		return "END";
	case ERR:
		return "ERR";

	default:
		return "How?";
	}
}

void printToken(Token t) {
	cout << "Token {" << endl;
	cout << "    type = " << typeToString(t.type) << endl;
	cout << "    val = \"" << t.value << "\"" << endl;
	cout << "    coord = (" << t.col << ',' << t.row << ')' << endl;
	cout << "}" << endl;
}

void testLexer(std::string fileName) {
	cout << "======== Starting InputStream/Lexer test ========" << endl;
	InputStream is(fileName);
	Lexer lex(&is);
	Token t = lex.read();
	int i = 0;
	while (t.type != END) {
		// if (t.type == ERR)
			printToken(t);
		i++;
		t = lex.read();
	}
	cout << i << endl;

}

string stringExpr(ExprStruct expr) {
	string res = "";
	res += "[" + expr.operand1.value + "]";
	if (expr.operation != "")
		res += " [" + expr.operation + "] [" + expr.operand2.value + "]";
	return res;
}

void printJump(StatementStruct *st) {
	Parser::JmpStruct jmp = *(Parser::JmpStruct *) st->ptr;
	cout << "JmpStruct {" << endl;
	cout << "  arg1: " << jmp.arg1 << endl;
	cout << "  arg2: " << jmp.arg2 << endl;
	cout << "  instruction: " << jmp.instruction << endl;
	cout << "  label: " << jmp.label << endl;
	cout << "};" << endl;
}

void printLoad(StatementStruct *st) {
	Parser::LoadStruct load = *(Parser::LoadStruct *) st->ptr;
	cout << "LoadStruct {" << endl;
	cout << "  reg: " << load.reg << endl;
	cout << "  size: " << load.load << endl;
	cout << "  expression: " << stringExpr(load.expr) << endl;
	cout << "  fromMemory: " << load.reference << endl;	
	cout << "};" << endl;
}

void printStore(StatementStruct *st) {
	Parser::StoreStruct store = *(Parser::StoreStruct *) st->ptr;
	cout << "StoreStruct {" << endl;
	cout << "  dest: " << stringExpr(store.dest) << endl;
	cout << "  val: " << store.val.value << endl;
	cout << "  size: " << store.store << endl;
	cout << "};" << endl;
}

void printConvert(StatementStruct *st) {
	Parser::ConvertStruct conv = *(Parser::ConvertStruct *) st->ptr;
	cout << "ConvertStruct {" << endl;	
	cout << "  dest: " << conv.destReg << endl;
	cout << "  source: " << conv.sourceReg << endl;
	cout << "  conversion: " << conv.conv << endl;
	cout << "};" << endl;
}

void printKeyword(StatementStruct *st) {
	Parser::KeyStruct kw = *(Parser::KeyStruct *) st->ptr;
	cout << "KeyStruct {" << endl;	
	cout << "  instruction: " << kw.instruction << endl;
	if (kw.value != "")
		cout << "  value: " << kw.value << endl;
	cout << "};" << endl;
}

void printStatement(StatementStruct *st) {
	switch (st->type) {
	case JUMP:
		printJump(st);
		break;
	case LOAD:
		printLoad(st);
		break;
	case STORE:
		printStore(st);
		break;
	case CONVERT:
		printConvert(st);
		break;
	case KEYWORD:
		printKeyword(st);
		break;
	
	default:
		break;
	}
}

void testParser(std::string fileName) {
	InputStream is(fileName);
	Lexer lex(&is);
	Parser parser(&lex);

	std::vector<StatementStruct*> v = parser.parse();
	for (int i = 0; i < v.size(); i++) {
		printStatement(v.at(i));
	}
}

void testCompiler(std::string fileName) {
	InputStream is(fileName);
	Lexer lex(&is);
	Parser parser(&lex);
	Compiler compiler;

	compiler.Compile(&parser);
}


void printError(std::string name, std::string error, Token t) { 
	std::cout << "Error" << name << ": " << error << " at (" << t.col << ',' << t.row << ")\n";
}

#define ERROR(name,error,tok)							\
{														\
	printError(name, error, tok);						\
	return false;										\
}

void tempo(string str1, string str2) {
	cout << str1 << "," << str2 << endl;
}

int main() {
	string fileName = "_test1.s";
	// testInputStream(fileName);
	// testLexer(fileName);
	// testParser(fileName);
	testCompiler(fileName);
}