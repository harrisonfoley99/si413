#include "sym.h"

//-- Globals
int cnt = 0;   //-- counter used to give unique ids to Sym's
vector<Sym*> symMem;

int Token::next(string s, int t)
{
  int n = cnt;
  Token *p = new Token();
  p->tokType = t;
  p->val = s;
  symMem.push_back(p);
  return n;
}

void symClear()
{
  for(int i = 0; i < cnt; ++i)
    delete symMem[i];
  symMem.clear(); cnt = 0;
}

void Token::dot(ostream &out) { out << id << " [ label=\"" << val << "\" ];" << endl; }
void Token::text(ostream &out) { out << val; }

int NonTerm::next(string t, int a, int b, int c)
{
  int n = cnt;
  NonTerm *p = new NonTerm();
  p->ntType = t;
  p->child.push_back(a);
  if (b != -1) p->child.push_back(b);
  if (c != -1) p->child.push_back(c);
  symMem.push_back(p);
  return n;
}
void NonTerm::dot(ostream &out) 
{
  out << id << " [ label=\"" << ntType << "\" ];" << endl;
  for(int i = 0; i < child.size(); ++i)
    out << id << " -> " << symMem[child[i]]->id << ";" << endl;
  for(int i = 0; i < child.size(); ++i)
    symMem[child[i]]->dot(out);
}

void NonTerm::text(ostream &out)
{
  out << "(" << ntType << (child.size() > 0 ? " " : "");
  for(int i = 0; i < child.size(); ++i) {
    out << (i > 0 ? " " : "");
    symMem[child[i]]->text(out);
  }
  out << ")";
}

int findStart() {
  int k = symMem.size()-1;
  while(k >= 0 && symMem[k]->isSTOP())
    --k;
  return k;
}

void dotGraph(ostream &out, int ID)
{
  int k = findStart();
  if (k == -1) return;
  out << "digraph G {" << endl;
  symMem[k]->dot(out);
  out << "}" << endl; 
}

void textGraph(ostream &out, int id)
{
  int k = findStart();
  if (k == -1) return;
  symMem[k]->text(out);
  out << endl;
}
