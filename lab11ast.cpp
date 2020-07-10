/* SI 413 Summer 2020
 * Lab 8
 * This file contains the implementations of longer methods in the
 * AST class hierarchy.
 * Foley 211926
 */

#include "ast.hpp"
//Evaluates a boolean and/or operation
Value BoolOp::eval(Frame* env) {
  Value leftV = left->eval(env);
  string leftT = tname(leftV.getType());
  if(leftT != "BOOL"){
    errout << "Type mismatch: expected BOOL, got " << leftT << endl;
    error = true;
  }
  else{ 
    bool l = leftV.tf(); 
    bool r;
    Value rightV;
    string rightT;
    
    switch(op) {
      case AND: 
        if(!l) return Value(false);
        rightV = right->eval(env);
        rightT = tname(rightV.getType());
        if(rightT == "BOOL"){
          r = rightV.tf();
          if(!r) return Value(false);
          return Value(true);
        }
        errout << "Type mismatch: expected BOOL, got " << rightT << endl;
        error = true;
        return Value();
      case OR: 
        if(l) return Value(true);
        rightV = right->eval(env);
        rightT = tname(rightV.getType());
        if(rightT == "BOOL"){
          r = rightV.tf();
          if(r) return Value(true);
          return Value(false);
        }
        errout << "Type mismatch: expected BOOL, got " << rightT << endl;
        error = true;
        return Value();
      default:
        errout << "Internal Error: Illegal boolean operator" << endl;
        error = true;
    }
  }
  return Value();
}

// Returns pointer to first Frame containing reference to "name", nullptr if none found
Frame* findFrame(const string &name, Frame* env){
  while((*env).count(name) == 0){
    env = env->getParent();
    if(env == nullptr) break;
  }
  return env;
}

//Evaluates a comparison
Value CompOp::eval(Frame* env) {
  Value leftV = left->eval(env);
  Value rightV = right->eval(env);
  string leftT = tname(leftV.getType());
  string rightT = tname(rightV.getType());
  if(leftT != "NUM"){
    error = true;
    errout << "Type mismatch: expected NUM, got " << leftT << endl;
  }
  else if(rightT != "NUM"){
    error = true;
    errout << "Type mismatch: expected NUM, got " << rightT << endl;
  }
  if(error) return Value();
  
  int l = leftV.num();
  int r = rightV.num();
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
Value Read::eval(Frame* env) {
  string in;
  cin >> in;
  const char *in2 = in.c_str();
  int val = atoi(in2);
  return Value(val);
}

//Resolves a value to a variable
Value Id::eval(Frame* env) {
  Frame* defined_in = findFrame(val, env);
  if(defined_in == nullptr){
    error = true;
    errout << "ERROR: No binding for variable " << val << endl;
    return Value();
  }
  return (*defined_in)[val];
}

// Evaluates an arithmetic operation
Value ArithOp::eval(Frame* env) {
  //int l = left->eval(env).num();
  //int r = right->eval(env).num();
  Value leftV = left->eval(env);
  Value rightV = right->eval(env);
  string leftT = tname(leftV.getType());
  string rightT = tname(rightV.getType());
  if(leftT != "NUM"){
    error = true;
    errout << "Type mismatch: expected NUM, got " << leftT << endl;
  }
  else if(rightT != "NUM"){
    error = true;
    errout << "Type mismatch: expected NUM, got " << rightT << endl;
  }
  if(error) return Value();

  int l = leftV.num();
  int r = rightV.num();
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

//Evaluates a function call
Value Funcall::eval(Frame* env) {

  Value cV = funexp->eval(env);
  string cT = tname(cV.getType());
  
  if(cT != "FUN"){
    error = true;
    errout << "Type mismatch: expected FUN, got " << cT << endl;
    return Value();
  }

  Closure c = cV.func();

  Value argument = arg->eval(env);//Ryan Mcgannon showed me why this needed to be a Value, not an int
  string arg_name = c.lamPtr->getVar();
  Frame* parenv = c.envPtr;

  Frame* funenv = new Frame(parenv);
  (*funenv)[arg_name] = argument;
  (*funenv)["ret"] = Value();

  c.lamPtr->getBody()->exec(funenv);

  return (*funenv)["ret"];
}

//Executes a new assignment
void NewStmt::exec(Frame* env) {
  if(error) return;
  Value v = rhs->eval(env);
  string id = lhs->getVal();
  if((*env).count(id) == 0){
    (*env)[id] = v;
    getNext()->exec(env);
    return;
  }
  error = true;
  errout << "ERROR: Variable " << id << " already bound!" << endl;
  return;
}

//Executes a re-assignment
void Asn::exec(Frame* env) {
  if(error) return;
  Value v = rhs->eval(env);
  string id = lhs->getVal();
  Frame* defined_in = findFrame(id, env);
  if(defined_in != nullptr){
    (*defined_in)[id] = v;
    getNext()->exec(env);
    return;
  }
  error = true;
  errout << "ERROR: Can't rebind " << id << "; not yet bound!" << endl;
  return;
}

//Executes an if/ifelse statement
void IfStmt::exec(Frame* env) {
  Value resV = clause->eval(env);
  string resT = tname(resV.getType());
  if(resT != "BOOL"){
    error = true;
    errout << "Type mismatch: expected BOOL, got " << resT << endl;
    return;
  }
  bool res = resV.tf();
  if(res) ifblock->exec(env);
  else elseblock->exec(env);
  getNext()->exec(env);
  return;
} 

//Executes a while loop statement
void WhileStmt::exec(Frame* env) {
  Value resV = clause->eval(env);
  string resT = tname(resV.getType());
  if(resT != "BOOL"){
    error = true;
    errout << "Type mismatch: expected BOOL, got " << resT << endl;
    return;
  }
  while(cont){
    body->exec(env);
  }
  getNext()->exec(env);
  return;
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
