#ifndef _SYM_SI413_
#define _SYM_SI413_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

//-- Globals
class Sym;
/************************************************
 * This file
 ************************************************/
extern int cnt;   //-- counter used to give unique ids to Sym's
extern vector<Sym*> symMem;

//-- Class hierarchy for symbols
class Sym 
{ 
 public:
   int id;
   virtual void dot(ostream &out) = 0;
   virtual void text(ostream &out) = 0;
   virtual bool isSTOP() { return false; }
   Sym() { id = cnt++; }
 };

class Token : public Sym
 {
 public:
   string val;
   int tokType;
   bool isSTOP() { return val == ";"; }
   void dot(ostream &out);
   void text(ostream &out);
   static int next(string s, int t);
 };

class NonTerm : public Sym
{
public:
  vector<int> child;
  string ntType;
  void dot(ostream &out);
  void text(ostream &out);
  static int next(string t, int a, int b=-1, int c=-1);
};

extern void dotGraph(ostream &out, int id = -1);
extern void textGraph(ostream &out, int id = -1);
extern void symClear();

#endif
