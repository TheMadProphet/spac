#ifndef _PARSER_H_
#define _PARSER_H_

#include "lexer.h"
#include <vector>

enum StatementType {JUMP, LOAD, STORE, CONVERT, KEYWORD, INCLUDE};
enum Section {DATA, BSS, TEXT};

struct ExprStruct {
	Token operand1;
	Token operand2;
	std::string operation;
	bool built;		// False only in case of expression parsing error

	ExprStruct() {
		operation = "";
	}

	ExprStruct(bool _built) {
		operation = "";
		built = _built;
	}
};

struct StatementStruct 	{
	StatementType type;
	int line;
	void *ptr;
};

class Parser {
public:
	Parser (Lexer *lexer);
	~Parser();
	
	std::vector<StatementStruct *> parse();

	struct parserContainter {
		std::vector<StatementStruct *> dataStatements;	// Initialized data
		std::vector<StatementStruct *> bssStatements;	//
		std::vector<StatementStruct *> textStatements;	// The code
		std::vector<StatementStruct *> otherStatements;	// Include, Extern, Global...
	};

	struct JmpStruct {
		std::string instruction;	// Branching intruction
		std::string arg1;			// Literal or number for comparison
		std::string arg2;			// Literal or number for comparison
		std::string label;			// Where to jump
	};

	struct LoadStruct {
		std::string reg;	// Register where the value will be loaded
		int load;			// Amount of bytes to load
		ExprStruct expr;	/* Expression which will be loaded in register. If reference is true, this
							 * is the address in memory from where we will load value into register. */
		bool reference;		// Are we loading from memory?

		LoadStruct() {
			reg = "";
			load = 0;
			expr = ExprStruct(false);
			reference = false;
		}
	};

	struct ConvertStruct {
		std::string destReg;		// Register where the converted number is stored
		std::string conv;			// Conversion function
		std::string sourceReg;		// This is the register which will be converted
	};	

	struct StoreStruct {
		ExprStruct dest; 		// Address where the value will be stored
		Token val;  			// Value that will be stored in memory
		int store;				// Amount of bytes to store
	};

	struct KeyStruct {
		std::string instruction;	// section, include, extern, global, syscall, call, ret
		std::string value;			// for keywords which require arguments
		
		KeyStruct() {
			value = "";
		}
	};

private:
	Lexer *lx;
	// List of all the parsed statements
	std::vector<StatementStruct *> statements;
	Section currentSection;

	// Prints error message and token's coordinates
	void printError(std::string name, std::string error, Token t);
	void printTokenLine(Token t);

	bool parseTextStatement(Token first);
	bool parseDataStatement(Token first);
	bool parseOtherStatement(Token tok);

	// Parses a jump/branch instruction
	bool parseJump();
	// Parses keyword
	bool parseKeyword();
	// Parses store-in-memory statement
	bool parseStore();

	// Returns assignment's size specifier, -1 on error.
	int assignmentSize(Token t);
	// Parses load in which convert is called
	bool parseLoadConvert(std::string dest);
	// Parses load in which source is number or register (or operation of either)
	bool parseLoadInstant(std::string dest);
	// Parses load in which the source is memory
	bool parseLoadReference(std::string dest);
	/* Parses load statement. This is more of a redirection function. 
	 * It will call other appropriate, more specific, load function. */
	bool parseLoad();

	// Parses simple 2 operand-only expression
	ExprStruct parseExpr();
	// Same as parseExpr() but only adding and substracting is allowed
	ExprStruct parseExpr(bool onlyAdditives);
};


#endif