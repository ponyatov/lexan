#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { glob_init(); yyparse(); 
	cout<<module.eval()->dump()<<"\n\n"; return 0; }

Sym::Sym(string T,string V,Env*E) { tag=T; val=V; env=new Env(E); }
Sym::Sym(string V):Sym("",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }

Sym* Sym::copy(Env*E) {
	Sym*R = new Sym(tag,val,E);
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		R->push((*it)->copy(R->env));
	return R; }

string Sym::tagval() { return "<"+tag+":"+val+">"; }
string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::dump(int depth) {
	string S = "\n"+pad(depth)+tagval()+env->dump();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym*E = env->lookup(val); if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++) (*it)=(*it)->eval();
	return this; }

Sym* Sym::eq(Sym*o) { glob.set(val,o); return o; }
Sym* Sym::at(Sym*o) { push(o); return this; }

List::List():Sym("","") {}

Op::Op(string V):Sym("op",V) {}
Sym* Op::eval() {
	if (val=="=") return nest[0]->eq(nest[1]->eval());
	else Sym::eval();
	if (val=="@") return nest[0]->at(nest[1]->eval()); 
	return this;
}

Lambda::Lambda():Sym("^","^") { env = new Env(&glob); }
Sym* Lambda::eval() { return this; }
Sym* Lambda::at(Sym*o) {
	Sym*L = copy(env);//nest[0]->copy(env);
	L->env->set(env->iron.begin()->first,o);
	return L->eval(); }

Env::Env(Env*X) { next=X; }
void Env::set(string V,Sym*o) { iron[V]=o; }
void Env::par(Sym*o) { set(o->val,o); }
string Env::dump() { string S;
	for (auto it=iron.begin(),e=iron.end();it!=e;it++)
		S += ","+it->first+"="+it->second->tagval();
	return S; }

Sym* Env::lookup(string V) {
	auto it=iron.find(V); if (it!=iron.end()) return it->second;
	else if (next) return next->lookup(V);
	else return NULL;
}

Env glob(NULL);
void glob_init(){ }//glob.iron["nil"] = new Sym("nil",""); }

