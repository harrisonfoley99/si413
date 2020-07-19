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

string BoolExp::eval(ostream& out, Context* con) {
  int toBePrinted = val ? 1 : 0;
  string dest = con->nextRegister();
  out << "    " << dest << " = sub i64 " << toBePrinted << ", 0" << endl;
  return dest;
}

string NegOp::eval(ostream& out, Context* con) {
  string r = right -> eval(out, con);
  string dest = con -> nextRegister();
  out << "    " << dest << " = mul i64 " << r << ", -1" << endl;
  return dest;
}

string CompOp::eval(ostream& out, Context* con) {
  string l = left -> eval(out, con);
  string r = right -> eval(out, con);
  string dest = con -> nextRegister();
  string dest2 = con -> nextRegister();
  out << "    " << dest << " = icmp ";
  switch(op) {
    case EQ:
      out << "eq ";
      break;
    case NE:
      out << "ne ";
      break;
    case LT:
      out << "slt ";
      break;
    case LE: 
      out << "sle ";
      break;
    case GT:
      out << "sgt "; 
      break;
    case GE:
      out << "sge ";
      break;
    default:
      // should never happen...
      errout << "Internal Error: Illegal comparison operator" << endl;
      exit(8);
  }
  out << " i64 " << l << ", " << r << endl;
  out << "    " << dest2 << " = zext i1 " << dest << " to i64" << endl;
  return dest2;
}

string BoolOp::eval(ostream& out, Context* con) {
  string l = left -> eval(out, con);
  string r = right -> eval(out, con);
  string dest = con -> nextRegister();
  out << "    " << dest << " = ";
  switch(op) {
    case AND:
      out << "and ";
      break;
    case OR:
      out << "or ";
      break;
    default:
      // should never happen...
      errout << "Internal Error: Illegal comparison operator" << endl;
      exit(8);
  }
  out << "i64 " << l << ", " << r << endl;
  return dest;
}

string NotOp::eval(ostream& out, Context* con) {
  string toBeNegated = right -> eval(out, con);
  string dest = con -> nextRegister();
  out << "    " << dest << " = xor i64 1 , " << toBeNegated << endl;
  return dest; 
}

string Id::eval(ostream& out, Context* con){
  string loadReg = con -> nextRegister();
  
  string varInScope = con->findVarInScope(val);
  if(varInScope=="ERROR"){
    errout << "ERROR: No binding for variable " << val << endl;
    exit(1);
  }

  //load business
  out << "    " << loadReg << " = load i64, i64* " << varInScope << endl;

  return loadReg;
}

// Evaluates a lambda expression
string Lambda::eval(ostream& out, Context* con) {
  //get needed stuff
  string lamReg = con -> nextRegister();
  string funAt = con -> nextFunc();
  string paramReg = con -> nextRegister();
  string argPtrReg = con -> nextRegister();
  string retPtrReg = con -> nextRegister();
  string retReg = con -> nextRegister();
  string paramName = getVar();
  ostringstream inception;

  //write stuff down that needs to be done now
  out << "    " << lamReg << " = ptrtoint i64(i64)* " << funAt << " to i64" << endl;

  //delay function definition
  inception << "define i64 " << funAt << " (i64 " << paramReg << ") {" << endl;
  
  //memory stuff
  inception << "    " << argPtrReg << " = alloca i64" << endl;
  inception << "    " << retPtrReg << " = alloca i64" << endl;
  inception << "    store i64 " << paramReg << ", i64* " << argPtrReg << endl;

  //scope stuff 
  map<string,string> funScope;
  funScope[paramName] = argPtrReg;
  funScope["ret"] = retPtrReg;
  con->scopeStack.push_back(funScope);

  //exec the body
  body->exec(inception, con);

  //return
  inception << "    " << retReg << " = load i64, i64* " << retPtrReg << endl;
  inception << "    ret i64 " << retReg << endl << "}" << endl;
  con->scopeStack.pop_back();
  //place function at and of delayout
  delayout << inception.str();
  //return
  return lamReg;
}

string Funcall::eval(ostream& out, Context* con) {
  string evaledArg = arg -> eval(out, con);
  string funNumReg = funexp -> eval(out, con);
  string funPtrReg = con -> nextRegister();
  string endReg = con -> nextRegister();
  out << "    " << funPtrReg << " = inttoptr i64 " << funNumReg << " to i64(i64)*" << endl;
  out << "    " << endReg << " = call i64(i64) " << funPtrReg << "(i64 " << evaledArg << ")" << endl; 
  return endReg;
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

void Asn::exec(ostream& out, Context* con){
  string id = lhs -> getVal();
  string valReg = rhs -> eval(out, con);

  string ptrReg = con->findVarInScope(id);
  if(ptrReg == "ERROR"){
    errout << "ERROR: Can't rebind " << id << "; not yet bound!" << endl;
    exit(1);
  }

  out << "    store i64 " << valReg << ", i64* " << ptrReg << endl;

  getNext()->exec(out, con);
}

void NewStmt::exec(ostream& out, Context* con){
  string id = lhs -> getVal();
  string valReg = rhs -> eval(out, con);
  string ptrReg = con -> nextRegister();

  //error check
  if(con->scopeStack.back().count(id) != 0){
    errout << "ERROR: Variable " << id << " already bound!" << endl;
    exit(1);
  } 

  //do assembly stuff
  out << "    " << ptrReg << " = alloca i64" << endl;
  out << "    store i64 " << valReg << ", i64* " << ptrReg << endl;

  //do map stuff
  con->scopeStack.back()[id] = ptrReg; 

  getNext()->exec(out, con);
}

void IfStmt::exec(ostream& out, Context* con){
  string ifLab = con -> nextLabel();
  string elseLab = con -> nextLabel();
  string endLab = con -> nextLabel();
  string cond = clause -> eval(out, con);
  string smolReg = con -> nextRegister();
  out << "    " << smolReg << " = trunc i64 " << cond << " to i1" << endl;
  out << "    br i1 " << smolReg << ", label %" << ifLab << ", label %" << elseLab << endl;
  out << ifLab << ":" << endl;
  ifblock -> exec(out, con);
  out << "    br label %" << endLab << endl << elseLab << ":" << endl;
  elseblock -> exec(out, con);
  out << "    br label %" << endLab << endl << endLab << ":" << endl;
  getNext() -> exec(out, con);
}

void WhileStmt::exec(ostream& out, Context* con){
  string loopLab = con -> nextLabel();
  string checkLab = con -> nextLabel();
  string endLab = con -> nextLabel();
  string cond;
  string smolReg = con -> nextRegister();
  out << "    br label %" << checkLab << endl << checkLab << ":" << endl;
  cond = clause -> eval(out, con);
  out << "    " << smolReg << " = trunc i64 " << cond << " to i1" << endl;
  out << "    br i1 " << smolReg << ", label %" << loopLab << ", label %" << endLab << endl;
  out << loopLab << ":" << endl;
  body -> exec(out, con);
  out << "    br label %" << checkLab << endl << endLab << ":" << endl;
  getNext() -> exec(out, con);
}

// execute a write statement by calling printf
void Write::exec(ostream& out, Context* con) {
  string r = val->eval(out, con);
  out << "    call i32(i8*,...) @printf("
    << "i8* getelementptr([5 x i8], [5 x i8]* @pfmt, i32 0, i32 0), "
    << "i64 " << r << ")" << endl;
  getNext()->exec(out, con);
}

//calls printf AND scanf 
string Read::eval(ostream& out, Context* con) {
  string scanReg = con -> nextRegister();
  out << "    " << scanReg << " = alloca i64" << endl;
  out << "    call i32(i8*,...) @printf("
    << "i8* getelementptr([6 x i8], [6 x i8]* @pfmt0, i32 0, i32 0), "
    << "i64* " << scanReg << ")" << endl;
  out << "    call i32(i8*,...) @scanf("
    << "i8* getelementptr([5 x i8], [5 x i8]* @pfmt, i32 0, i32 0), "
    << "i64* " << scanReg << ")" << endl;
  string dest = con -> nextRegister();
  out << "    " << dest << " = load i64, i64* " << scanReg << endl;
  return dest;
}
