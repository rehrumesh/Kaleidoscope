#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

//----------------------
// Lexer
//----------------------

enum Token {
	tok_eof = -1,
	tok_def = -2,
	tok_extern = -3,
	tok_identifier = -4,
	tok_number = -5
};

static std::string IdentifierStr;
static double NumVal;

static int gettok(){
	static int LastChar;

	while(isspace(LastChar))
		LastChar = getchar();

	if(isalpha(LastChar)){
		IdentifierStr = LastChar;
		while(isalnum((LastChar = getchar())))
			IdentifierStr += LastChar;

		if(IdentifierStr == "def") return tok_def;
		if(IdentifierStr == "extern") return tok_extern;
		return tok_identifier;
	}

	if(isdigit(LastChar) || LastChar == '.'){
		std::string NumStr;
		do{
			NumStr += LastChar;
			LastChar = getchar();
		} while(isdigit(LastChar) || LastChar = '.');

		NumVal = strtod(NumStr.c_str(),0);
		return tok_number;
	}

	if(LastChar == "#"){
		do LastChar = getchar();
		while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');

		if(LastChar != EOF)
			return gettok();
	}

	if (LastChar == EOF)
		return tok_eof;

	int ThisChar = LastChar;
	LastChar = getchar();
	return ThisChar;
}

//-------------------------
// AST
//-------------------------
class ExprAST{
public:
	virtual ~ExprAST(){}
};

class NumberExprAST : public ExprAST{
	double Val;
public:
	NumberExprAST(double val) : Val(val){}
};

class VariableExprAST : public ExprAST{
	std::string Name;
public:
	VariableExprAST(const std::string &name) : Name(name){}
};

class BinaryExprAST : public ExprAST{
	char Op;
	ExprAST *LHS, *RHS;
public:
	BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
		: Op(op), LHS(lhs), RHS(rhs){}
};

class CallExprAST : public ExprAST{
	std::string Callee;
	std::vector<ExperAST*> Args;
public:
	CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
		: Callee(callee), Args(args){}
};

class PrototypeAST{
	std::string Name;
	std::vector<std::string> Args;
public:
	PrototypeAST(const std::string &name, const std::vector<std::string> &args)
    		: Name(name), Args(args) {}
};

class FunctionAST{
	PrototypeAST *Proto;
	ExprAST *Body;
public:
	FunctionAST(PrototypeAST *proto, ExprAST *body)
    		: Proto(proto), Body(body) {}
};


static int CurTok;
static int getNextToken(){
	return CurTok = gettok();
}

/// Error* - These are little helper functions for error handling.
ExprAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
PrototypeAST *ErrorP(const char *Str) { Error(Str); return 0; }
FunctionAST *ErrorF(const char *Str) { Error(Str); return 0; }
