#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { glob_init(); yyparse(); 
	cout<<module.eval()->dump()<<"\n\n"; return 0; }

Sym::Sym(string T,string V) { tag=T; val=V; env=new Env(NULL); }//&glob; }
Sym::Sym(string V):Sym("",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::tagval() { return "<"+tag+":"+val+">"; }
string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::dump(int depth) {
	string S = "\n"+pad(depth)+tagval()+env->dump();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	for (auto it=nest.begin(),e=nest.end();it!=e;it++) (*it)=(*it)->eval();
	return this; }

Sym* Sym::eq(Sym*o) { env->set(val,o); return o; }

List::List():Sym("","") {}

Op::Op(string V):Sym("op",V) {}
Sym* Op::eval() {
	if (val=="=") return nest[0]->eq(nest[1]->eval());
	Sym*E = env->lookup(val); if (E) return E; else Sym::eval();
	return this;
}

Lambda::Lambda():Sym("^","^") { env = new Env(&glob); }
Sym* Lambda::eval() { return this; }

Env::Env(Env*X) { next=X; }
void Env::set(string V,Sym*o) { iron[V]=o; }
void Env::par(Sym*o) { set(o->val,o); }
string Env::dump() { string S;
	for (auto it=iron.begin(),e=iron.end();it!=e;it++)
		S += ","+it->first;
	return S; }

Sym* Env::lookup(string V) {
	auto it=iron.find(V); if (it!=iron.end()) return it->second;
	else if (next) return next->lookup(V);
	else return NULL;
}

Env glob(NULL);
void glob_init(){ }//glob.iron["nil"] = new Sym("nil",""); }

