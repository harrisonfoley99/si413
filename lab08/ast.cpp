/* SI 413 Summer 2020
 * Lab 8
 * This file contains the implementations of longer methods in the
 * AST class hierarchy.
 * Foley 211926
 */

#include "ast.hpp"
//Evaluates a boolean and/or operation
Value BoolOp::eval() {
  bool l = left->eval().tf();
  bool r = right->eval().tf();
  switch(op) {
    case AND: return Value(l && r);
    case OR: return Value(l || r);
    default:
      errout << "Internal Error: Illegal boolean operator" << endl;
  }
  return Value();
}

//Executes a re-assignment
void Asn::exec() {
  Value v = rhs->eval();
  string id = lhs->getVal();
  if(symTab.count(id) != 0){
    symTab[id] = v;
    return;
  }
  error = true;
  errout << "ERROR: Can't rebind " << id << "; not yet bound!" << endl;
  return;
}

//Executes a new assignment
void NewStmt::exec() {
  Value v = rhs->eval();
  string id = lhs->getVal();
  if(symTab.count(id) == 0){
    symTab[id] = v;
    return;
  }
  error = true;
  errout << "ERROR: Variable " << id << " already bound!" << endl;
  return;
}

//Evaluates a comparison
Value CompOp::eval() {
  int l = left->eval().num();
  int r = right->eval().num();
  switch(op) {
    case LT: return Value(l < r);
    case GT: return Value(l > r);
    case LE: return Value(l <= r);
    case GE: return Value(l >= r);
    case EQ: return Value(l == r);
    case NE: return Value(l != r);
    default:
      error = true;
      errout << "Internal Error: Illegal comparison operator" << endl;
  }
  return Value();
}

//Evaluates a read operation
Value Read::eval() {
  string in;
  cin >> in;
  const char *in2 = in.c_str();
  int val = atoi(in2);
  return Value(val);
}

//Resolves a value to a variable
Value Id::eval() {
  if(symTab.count(val) == 0){
    error = true;
    errout << "ERROR: No binding for variable " << val << endl;
    return Value();
  }
  return symTab[val];
}

// Evaluates an arithmetic operation
Value ArithOp::eval() {
  int l = left->eval().num();
  int r = right->eval().num();
  switch(op) {
    case ADD: return Value(l + r);
    case SUB: return Value(l - r);
    case MUL: return Value(l * r);
    case DIV:
      if (r != 0) return Value(l / r);
      else if (!error) {
        error = true;
        errout << "ERROR: Divide by zero" << endl;
      }
      break;
    default:
      // should never happen...
      errout << "Internal Error: Illegal arithmetic operator" << endl;
  }
  return Value();
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


/*****************************************************
 * These functions only pertain to drawing AST diagrams
 * so you can safely ignore them.
 *****************************************************/

/* Adds this node and all children to the output stream in DOT format.
 * nextnode is the index of the next node to add. */
void AST::addToDot(ostream& out, int& nextnode) {
  int root = nextnode;
  ++nextnode;
  out << "\tn" << root << " [label=\"" << nodeLabel << "\"];" << endl;
  for (int i=0; i < children.size(); ++i) {
    int child = nextnode;
    children[i]->addToDot(out, nextnode);
    out << "\tn" << root << " -> n" << child << ";" << endl;
  }
}

/* Writes this AST to a .dot file as named. */
void AST::writeDot(const string& fname) {
  ofstream fout(fname);
  int nodes = 1;
  fout << "digraph AST {" << endl;
  addToDot (fout, nodes);
  fout << "}" << endl;
  fout.close();
}
