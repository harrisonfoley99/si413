/* SI 413 Summer 2020
 * Lab 11
 * This file contains the implementations of longer methods in the
 * AST class hierarchy.
 */

#include "ast.hpp"

// Evaluates an arithmetic operation
Value ArithOp::eval(Frame* env) {
  int l = left->eval(env).num();
  int r = right->eval(env).num();
  if (error) return Value();
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
  if (! a->hasNext()) { delete a; return b; }
  Stmt* last = a;
  while (last->getNext()->hasNext()) last = last->getNext();
  Stmt* ns = last->getNext();
  last->setNext(b);
  delete ns;
  return a;
}

// Default constructor for Stmt.
// This HAS to be declared here because it uses NullStmt, which
// hasn't been defined yet in the header file!
Stmt::Stmt() {
  next = new NullStmt();
  children.push_back(next);
}

void mkBuiltin(Stmt* p, const string& name, Frame* env, vector<AST*> &store) {
  Lambda* lp = new Lambda(new Id("x"),p);
  store.push_back(lp);
  Closure c = {lp,env};
  (*env)[name] = Value(c);
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
