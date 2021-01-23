#include "parser.h"
#include "types.h"

#define ERROR(name,error,tok)							\
{														\
	printError(name, error, tok);						\
	return false;										\
}

#define ASSERT(str1,str2, t)										\
{																	\
	if (str1 != str2) {												\
		std::cout << "Expected \"" << str1 << "\" ";				\
		std::cout << "but received \"" << str2 << "\" ";			\
		printTokenLine(t);											\
		return false;												\
	}																\
}


Parser::Parser(Lexer *lexer) {
	lx = lexer;
	statements.clear();
	currentSection = TEXT;
}

Parser::~Parser() {
}

void Parser::printError(std::string name, std::string error, Token t) { 
	std::cout << "ErrorParse" << name << ": " << error << " ";
	printTokenLine(t);
	std::cout << " \"" << t.value << "\"" << std::endl;
}

void Parser::printTokenLine(Token t) {
	std::cout << "at line " << t.row << ':' << t.col;
}

ExprStruct Parser::parseExpr() {
	return parseExpr(false);
}

ExprStruct Parser::parseExpr(bool onlyAdditives) {
	// First operand
	Token operand1 = lx->read();
	if (!isLoaded(operand1.type)) {
		printError("Expr", "Expected literal or register", operand1);
		return ExprStruct(false);
	}

	// Operation
	Token operation = lx->peek();
	// If this token isn't operator, we assume expression is a single number or register
	if (operation.type != OP) {
		ExprStruct res = ExprStruct(true);
		res.operand1 = operand1;
		return res;
	} else if (onlyAdditives && !isAdditive(operation.value)) {
		std::cout << "ErrorExpr: Only adding and substracting is allowed here " << std::endl;
		printTokenLine(lx->read());
		return ExprStruct(false);
	}
	lx->read();
	// Second operand
	Token operand2 = lx->read();
	if (!isLoaded(operand2.type)) {
		printError("Expr", "Expected literal or register", operand2);
		return ExprStruct(false);
	}

	// Build structure and return
	ExprStruct res = ExprStruct(true);
	res.operand1 = operand1;
	res.operand2 = operand2;
	res.operation = operation.value;
	return res;
}

bool Parser::parseJump() {	
	int line = lx->peek().row;
	Token instruction = lx->read();

	// First argument
	Token arg1 = lx->read();
	if (!isLoaded(arg1.type))
		ERROR("Jump", "Only literals and registers are accepted when comparing", arg1);

	Token punc = lx->read();  // Read ','
	ASSERT(punc.value, ",", punc);

	// Second argument
	Token arg2 = lx->read();
	if (!isLoaded(arg2.type))
		ERROR("Jump", "Only literals and registers are accepted when comparing", arg2);

	punc = lx->read();	// Read ','
	ASSERT(punc.value, ",", punc);

	Token label = lx->read();
	// if label...

	// Build structure and return
	StatementStruct *res = new StatementStruct;
	JmpStruct *ptr = new JmpStruct;
	res->type = JUMP;
	res->ptr = ptr;
	res->line = line;

	ptr->arg1 = arg1.value;
	ptr->arg2 = arg2.value;
	ptr->instruction = instruction.value;
	ptr->label = label.value;

	statements.push_back(res);
	return true;
}

bool Parser::parseKeyword() {
	int line = lx->peek().row;
	StatementStruct *res = new StatementStruct();
	res->type = KEYWORD;

	KeyStruct *ptr = new KeyStruct;
	Token instruction = lx->read();
	ptr->instruction = instruction.value;
	res->ptr = ptr;
	res->line = line;

	// If its needs arguemnt, add it to structure
	if (keywordNeedsArgument(instruction.value)) {
		Token tok = lx->read();
		if (tok.value != "<") {
			// TODO: if its identifier..
			ptr->value = tok.value;

			if (strEqu(tok.value, "DATA")) {
				currentSection = DATA;
			} else if (strEqu(tok.value, "TEXT")) {
				currentSection = TEXT;
			}
		}

		ASSERT(tok.value, "<", tok);

		tok = lx->read();
		// TODO: if its identifier..
		ptr->value = tok.value;

		tok = lx->read();
		ASSERT(tok.value, ">", tok);
	}

	statements.push_back(res);
	return true;
}

int Parser::assignmentSize(Token t) {
	if (t.type != OP) {
		printError("Operand", "Expected assignment", t);
		return -1;
	}
	std::string operand = t.value;
	if (operand == "=")
		return 4;
	else {
		if (operand.length() == 2) {
			printError("Operand", "Operation size not specified", t);
			return -1;
		}
		int load = operand[2] - '0';
		if (operand.length() > 3 || (load != 1 && load != 2)) {
			printError("Operand", "Operation size specifier must be 1('=.1') or 2('=.2')", t);
			return -1;
		}
		return load;
	}
	return -1;
}

bool Parser::parseLoadConvert(std::string dest) {
	int line = lx->peek().row;
	// Read assign operand and check for errors
	std::string operand = lx->read().value;
	if (operand.length() > 1 && operand[1] == '.')
		ERROR("Convert", "Load size mustn't be specified when converting", lx->peek());

	// The conversion function
	std::string conv = lx->read().value;
	
	// Read source register and check for errors
	Token source = lx->read();
	if (source.type != REG)
		ERROR("Convert", "Only registers can be used during conversions", source);
	
	// Build structure and return
	StatementStruct *res = new StatementStruct();
	ConvertStruct *ptr = new ConvertStruct;
	res->type = CONVERT;
	res->ptr = ptr;
	res->line = line;

	ptr->destReg = dest;
	ptr->conv = conv;
	ptr->sourceReg = source.value;
	
	statements.push_back(res);
	return true;
}

bool Parser::parseLoadInstant(std::string dest) {
	int line = lx->peek().row;
	int size = assignmentSize(lx->read());
	if (size == -1) return false; // Exit if we encountered error

	ExprStruct expr = parseExpr();

	/*
	// If no size is specified, just parse the expression
	if (size == 4)
		expr 
	// Otherwise the source is number or register (no operation allowed)
	else {
		Token cur = lx->read();
		Token next = lx->peek();
		if (!isLoaded(cur.type))
			ERROR("Load", "Expected literal, register or memory address", cur);

		if (isLoaded(cur.type) && next.type == OP)
			ERROR("Load", "When specifying load size, no operations are allowed", next);

		expr = ExprStruct(true);
		expr.operand1 = cur.value;
		expr.operand2 = "";
		expr.operation = "";
	}*/

	// Now ready up the variables
	StatementStruct *res = new StatementStruct();
	LoadStruct *ptr = new LoadStruct;
	res->type = LOAD;
	res->ptr = ptr;
	res->line = line;

	// Set structure variables
	ptr->reg = dest;
	ptr->load = size;
	ptr->expr = expr;
	ptr->reference = false;

	// Save and return true for success
	statements.push_back(res);
	return true;
}

bool Parser::parseLoadReference(std::string dest) {	
	int line = lx->peek().row;
	int size = assignmentSize(lx->read());
	if (size == -1) return false; // Exit if we encountered error

	lx->read();				// Read 'M'
	Token tok = lx->read(); // Read '['
	ASSERT("[", tok.value, tok);

	ExprStruct expr = parseExpr(true);
	if (!expr.built) return false;

	tok = lx->read(); 		// Read ']'
	ASSERT("]", tok.value, tok);

	// Now ready up the variables
	StatementStruct *res = new StatementStruct();
	LoadStruct *ptr = new LoadStruct;
	res->type = LOAD;
	res->ptr = ptr;
	res->line = line;

	// Set structure variables
	ptr->reg = dest;
	ptr->load = size;
	ptr->expr = expr;
	ptr->reference = true;

	statements.push_back(res);
	return true;
}

bool Parser::parseLoad() {
	std::string dest = lx->read().value; // Register in which we will be loading value

	// Checking making sure that its followed by an assignment
	Token tok = lx->peek();
	if (tok.value[0] != '=')
		ERROR("Load", "Expected assignment", tok);

	// Grab next token and parse different variants of load
	tok = lx->peekExtra();
	if (tok.type == CONV)   return parseLoadConvert(dest);
	if (tok.type == REF)    return parseLoadReference(dest);
	if (isLoaded(tok.type)) return parseLoadInstant(dest);

	std::cout << "UnexpectedErrorLoad" << std::endl;
	return false;
}

bool Parser::parseStore() {
	int line = lx->peek().row;
	lx->read();		// Skip 'M'

	Token tok = lx->read();    // Should be '['
	ASSERT(tok.value, "[", tok);

	// Parse the address
	ExprStruct dest = parseExpr(true);
	if (!dest.built) return false;

	tok = lx->read(); 		// Should be ']'
	ASSERT(tok.value, "]", tok);

	Token assignment = lx->read();
	int store = assignmentSize(assignment);
	if (store == -1) return false;
	
	// Value that will be stored in memory
	Token val = lx->read();
	if (!isLoaded(val.type))
		ERROR("Store", "Expected literal or register", val);
	if (lx->peek().value != ";") 
		ERROR("Store", "No additional operations are allowed here", lx->read());

	// Build structure and return
	StatementStruct *res = new StatementStruct();
	StoreStruct *ptr = new StoreStruct();
	res->ptr = ptr;
	res->type = STORE;
	res->line = line;

	ptr->dest = dest;
	ptr->store = store;
	ptr->val = val;
	
	statements.push_back(res);
	return true;
}

bool Parser::parseDataStatement(Token tok) {

}

bool Parser::parseTextStatement(Token tok) {
	switch (tok.type) {
		case JMP:
			return parseJump();
		case KW:
			return parseKeyword();
		case REG:
			return parseLoad();
		case REF:
			return parseStore();
		
		default:
			printError("Unexpected", "Unexpected error", tok);
			if (tok.value[0] == 'R')
				std::cout << "Possible solution: Make sure to use registers up to"
				<< " 8: R[1-8]" << std::endl;
			return false;
	}
}

bool Parser::parseOtherStatement(Token tok) {
	lx->read();
	if (tok.type == KW) {
		// section, include, extern, global
		if (strEqu(tok.value, "SECTION")) {
			Token val = lx->read();
			if (val.type == KW) {
				if (strEqu(val.value, "DATA")) {
					currentSection = DATA;
				} else if (strEqu(val.value, "BSS")) {
					currentSection = BSS;

				} else if (strEqu(val.value, "TEXT")) {
					currentSection = TEXT;

				}

			}
		} else if (tok.value == "include") {
			
		}
	} else {
		printError("Parse", "Undefined identifier", tok);
	}
}

std::vector<StatementStruct*> Parser::parse() {
	while (true) {
		Token first = lx->peek();
		while (first.value == ";") {
			lx->read();
			first = lx->peek();
		}
		if (first.type == END) {
			// finish
		}

		bool success;
		if (currentSection == TEXT) {
			success = parseTextStatement(first);
		} else if (currentSection == BSS) {

		} else if (currentSection == DATA) {
			success = parseDataStatement(first);
		} else {
			success = parseOtherStatement(first);
		}

		if (success) {
			if (first.type != KW && lx->peek().value != ";")
				printError("", "Expected ';' symbol", lx->peek());
		} else {
			Token tok = lx->peek();
			while (tok.value != ";") {
				lx->read();
				tok = lx->peek();
			}
		}
	}
}