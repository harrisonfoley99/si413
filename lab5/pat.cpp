/*** This is file: pat.cpp ***/
#include "pat.h"
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

map<string, vector<string> > T;
int nextTok = -1; // store next token
void S();
void seq();
void stail();
void cseq();
void ctail();
void pseq();
void ptail();
void aseq();

//-- Helper functions
void paerror(const string& str = "") { // can be called with no arguments! 
  cerr << "Parse error!" << endl; 
  exit(2); }
int peek() { 
  if (nextTok == -1) nextTok = yylex(); 
  return nextTok; }
void match(int tok) { 
  if (tok == peek()) nextTok = -1; 
  else  paerror(); }

int main() {
  S();
  if(peek() != 0){
    cout << "Still input..." << endl;
  }
  return 0;
}

void S(){
  int p = peek();
  if(p == NAME || p == ATOM || p == LB){
    seq(); match(STOP); S();
  }
  else if(p==0){
    return;
  }
    
}

void seq(){
  int p = peek();
  if(p == NAME || p == ATOM || p == LB){
    cseq(); stail();
  }
  else paerror();
  return;
}

void stail(){
  int p = peek();
  if(p == FOLD){
    match(FOLD); cseq(); stail();
  }
  else if(p == STOP || p == RB){
    return;
  }
  else paerror();
  return;
}

void cseq(){
  int p = peek();
  if(p == NAME || p == ATOM || p == LB){
    pseq(); ctail();
  }
  else paerror();
  return;
}

void ctail(){
  int p = peek();
  if(p == NAME || p == ATOM || p == LB){
    pseq(); ctail();
  }
  else if(p == STOP || p == FOLD || p == RB){
    return;
  }
  else paerror();
  return;
}

void pseq(){
  int p = peek();
  if(p == NAME || p == ATOM || p == LB){
    aseq(); ptail();
  }
  else paerror();
  return;
}

void ptail(){
  int p = peek();
  if(p == COLON){
    match(COLON); match(NAME); ptail();
  }
  else if(p == POP){
    match(POP); ptail();
  }
  else if(p == STOP || p == FOLD || p == NAME || p == ATOM || p == LB || p == RB){
    return;
  }
  else paerror();
  return;
}

void aseq(){
  int p = peek();
  if(p == ATOM){
    match(ATOM);
  }
  else if(p == NAME){
    match(NAME);
  }
  else if(p == LB){
    match(LB); seq(); match(RB);
  }
  else paerror();
  return;
}
