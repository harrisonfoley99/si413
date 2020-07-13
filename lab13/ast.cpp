/* SI 413 Fall 2019
 * Lab 13
 * This file contains the implementations of longer methods in the
 * AST class hierarchy.
 */

#include "ast.hpp"

// Evaluates an arithmetic operation
string ArithOp::eval(ostream& out, Context* con) {
  string l = left->eval(out, con);
  string r = right->eval(out, con);
  string dest = con->nextRegister();
  out << "    " << dest << " = ";
  switch(op) {
    case ADD:
      out << "add";
      break;
    case SUB:
      out << "sub";
      break;
    case MUL:
      out << "mul";
      break;
    case DIV:
      out << "sdiv";
      break;
    default:
      // should never happen...
      errout << "Internal Error: Illegal arithmetic operator" << endl;
      exit(8);
  }
  out << " i64 " << l << ", " << r << endl;
  return dest;
}

// Appends b to the end of a and returns the result.
Stmt* Stmt::append(Stmt* a, Stmt* b) {
  if (! a->hasNext()) return b;
  Stmt* last = a;
  while (last->getNext()->hasNext()) last = last->getNext();
  last->setNext(b);
  return a;
}

// Default constructor for Stmt.
// This HAS to be declared here because it uses NullStmt, which
// hasn't been defined yet in the header file!
Stmt::Stmt() {
  next = new NullStmt();
  children.push_back(next);
}

// execute a write statement by calling printf
void Write::exec(ostream& out, Context* con) {
  string r = val->eval(out, con);
  out << "    call i32(i8*,...) @printf("
    << "i8* getelementptr([5 x i8], [5 x i8]* @pfmt, i32 0, i32 0), "
    << "i64 " << r << ")" << endl;
  getNext()->exec(out, con);
}
