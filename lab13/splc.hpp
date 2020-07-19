/* SI413 Fall 2018
 * Lab 13
 * Global definitions for the compiler
 * Harrison Foley
 */

#ifndef SPLC_HPP
#define SPLC_HPP

#include "colorout.hpp"
#include <vector>
#include <map>
#include <sstream>
using namespace std;

// forward declarations
class Stmt;
class Lambda;

// Global variables
extern colorout resout;
extern colorout errout;
extern stringstream delayout;
extern stringstream global_declarations;
extern Stmt* tree;

// This class holds the context for AST evaluation
// Right now, it just keeps track of register or label names to use
class Context {
private:
  int regcount = 0;
  int branchcount = 0;
  int funccount = 0;
public:  
  // returns a series of unique register names like
  // "%v1", "%v2", etc.
  string nextRegister() {
    return "%v" + to_string(++regcount);
  }
  string nextLabel(){
    return "b" + to_string(++branchcount);
  }
  string nextFunc(){
    return "@f" + to_string(++funccount);
  }
  vector<map<string,string>> scopeStack;

  string findVarInScope(string var){
    for(int i = scopeStack.size()-1; i >= 0; i--){
      if(scopeStack[i].count(var) > 0){
        return scopeStack[i][var];
      }
    }
    //I can do this because all "var names" HAVE to be registers as the prog is written.
    return "ERROR";
  }
};


#endif // SPLC_HPP
