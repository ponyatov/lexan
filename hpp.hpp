#ifndef _H_LEXAN
#define _H_LEXAN

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym;
struct Env {
	map<string,Sym*> iron;
	Env(Env*); Env* next;
	Sym* lookup(string);
	void set(string,Sym*);
	void par(Sym*);
	string dump();
};
extern Env glob;
extern void glob_init();
struct Sym {
	string tag,val;
	Sym(string,string); Sym(string); Sym* copy(Env*);
	vector<Sym*> nest; void push(Sym*); void pop();
	Env* env;
	string dump(int depth=0); string pad(int);
	virtual string tagval(); string tagstr();
	virtual Sym* eval();
	virtual Sym* eq(Sym*);
	virtual Sym* at(Sym*);
};

struct List: Sym { List(); };

struct Op: Sym { Op(string); Sym*eval(); };

struct Lambda: Sym { Lambda(); Sym*eval(); Sym*at(Sym*); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
extern Sym module;
#include "ypp.tab.hpp"

#endif // _H_LEXAN
