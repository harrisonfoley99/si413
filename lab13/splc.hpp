/* SI413 Fall 2018
 * Lab 13
 * Global definitions for the compiler
 * YOUR NAME HERE
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
extern Stmt* tree;

// This class holds the context for AST evaluation
// Right now, it just keeps track of register or label names to use
class Context {
private:
  int regcount = 0;
  
public:  
  // returns a series of unique register names like
  // "%v1", "%v2", etc.
  string nextRegister() {
    return "%v" + to_string(++regcount);
  }
};

#endif // SPLC_HPP
