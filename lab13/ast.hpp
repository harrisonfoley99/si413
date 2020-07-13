/* SI 413 Fall 2019
 * Lab 13
 * This is a C++ header file for the AST class hierarchy.
 * YOUR NAME HERE
 */

#ifndef AST_HPP
#define AST_HPP

#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "splc.hpp"
using namespace std;

#include "colorout.hpp"

// Declare the output streams to use everywhere
extern colorout resout;
extern colorout errout;

// This enum type gives codes to the different kinds of operators.
// Basically, each oper below such as DIV becomes an integer constant.
enum Oper { ADD=0, SUB, MUL, DIV, LT, GT, LE, GE, EQ, NE, AND, OR, NOT };
static string oplab[] = { "+","-","*","/","<",">","<=",">=","=","!=","and","or","not"}; 

// These are forward declarations for the classes defined below.
// They show the class hierarchy.
class AST;
class Stmt;
class NullStmt;
class Block;
class IfStmt;
class WhileStmt;
class NewStmt;
class Asn;
class Write;
class Exp;
class Id;
class Num;
class BoolExp;
class ArithOp;
class CompOp;
class BoolOp;
class NegOp;
class NotOp;
class Read;
class Lambda;
class Funcall;

/* The AST class is the super-class for abstract syntax trees.
 * Every type of AST (or AST node) has its own subclass.
 */
class AST {
private:
  /* Adds this node and all children to the output stream in DOT format.
   * nextnode is the index of the next node to add. */
  void addToDot(ostream& out, int& nextnode);

protected:
  // These two protected fields determine the structure of the AST.
  string nodeLabel;
  vector<AST*> children;

  // Inserts a new AST node as a child of this one.
  // (where the new node is inserted depends on which subclass.)
  virtual void ASTchild(AST* child) = 0;

public:
  /* Writes this AST to a .dot file as named. */
  void writeDot(const string& fname);

  /* Makes a new "empty" AST node. */
  AST() { nodeLabel = "EMPTY"; }

  /* Deallocates memory for this AST note and its children. */
  virtual ~AST() {
    for (AST* child : children) delete child;
    children.clear();
  }
};

/* Every AST node that is not a Stmt is an Exp.
 * These represent actual computations that return something
 */
class Exp :public AST {
protected:
  // Inserts a new AST as a child of this one.
  void ASTchild(AST* child) override { children.push_back(child); }

public:
  /* This is the method that must be overridden by all subclasses.
     It should emit LLV code to evaluate this expression, and return
     a string that either is the string representation of the i64 
     value of the expression, or is an LLVM variable name that will 
     contain the value of the expression after it is evaluated 
     during LLVM execution.
  */
  virtual string eval(ostream& out, Context* con) {
    errout << "eval() not yet implemented for "
	   << nodeLabel << " nodes!" << endl;
    return "";
  }
};

/* An identifier, i.e. variable or function name. */
class Id :public Exp {
private:
  string val;

public:
  // Constructor from a C-style string
  Id(const char* v) {
    val = v;
    nodeLabel = "Exp:Id:" + val;
  }

  // Returns a reference to the stored string value.
  string& getVal() { return val; }
};

/* A literal number in the program. */
class Num :public Exp {
private:
  int val;

public:
  Num(int v) {
    val = v;
    // Converting integers to strings is a little annoying...
    ostringstream label;
    label << "Exp:Num:" << val;
    nodeLabel = label.str();
  }

  // To evaluate, just return the number!
  virtual string eval(ostream& out, Context* con) {
    return to_string(val);     
  }
};

/* A literal boolean value like "true" or "false" */
class BoolExp :public Exp {
private:
  bool val;

public:
  BoolExp(bool v) {
    val = v;
    nodeLabel = "Exp:Bool:";
    if (v) nodeLabel += "true";
    else nodeLabel += "false";
  }
};

/* A binary opration for arithmetic, like + or *. */
class ArithOp :public Exp {
private:
  Oper op;
  Exp* left;
  Exp* right;

public:
  ArithOp(Exp* l, Oper o, Exp* r) {
    op = o; left = l; right = r;
    nodeLabel = "Exp:ArithOp:" + oplab[op];
    ASTchild(left);
    ASTchild(right);
  }

  string eval(ostream& out, Context* con) override;
};

/* A binary operation for comparison, like < or !=. */
class CompOp :public Exp {
private:
  Oper op;
  Exp* left;
  Exp* right;

public:
  CompOp(Exp* l, Oper o, Exp* r) {
    op = o; left = l; right = r;
    nodeLabel = "Exp:CompOp:" + oplab[op];
    ASTchild(left);
    ASTchild(right);
  }
};

/* A binary operation for boolean logic, like "and". */
class BoolOp :public Exp {
private:
  Oper op;
  Exp* left;
  Exp* right;

public:
  BoolOp(Exp* l, Oper o, Exp* r) {
    op = o; left = l; right = r;
    nodeLabel = "Exp:BoolOp:" + oplab[op];
    ASTchild(left);
    ASTchild(right);
  }
};

/* This class represents a unary negation operation. */
class NegOp :public Exp {
private:
  Exp* right;

public:
  NegOp(Exp* r) {
    nodeLabel = "Exp:NegOp";
    right = r;
    ASTchild(right);
  }
};

/* This class represents a unary "not" operation. */
class NotOp :public Exp {
private:
  Exp* right;

public:
  NotOp(Exp* r) {
    nodeLabel = "Exp:NotOp";
    right = r;
    ASTchild(right);
  }
};

/* A read expression. */
class Read :public Exp {
public:
  Read() { nodeLabel = "Exp:Read"; }
};

/* A Stmt is anything that can be evaluated at the top level such
 * as I/O, assignments, and control structures.
 * The last child of any statement is the next statement in sequence.
 */
class Stmt :public AST {
private:
  // Pointer to the next statement in sequence.
  Stmt* next;

protected:
  // Inserts a new AST as a child of this one.
  void ASTchild(AST* child) override {
    // This inserts before the last thing in the vector,
    // i.e., just before the "next" statement
    children.insert(children.end()-1, child);
  }

public:
  /* This static method is for building sequences of statements by the
   * parser. It takes two statements, and appends one at the end of the other.
   * The returned value is a pointer to the new statement representing
   * the sequence.
   */
  static Stmt* append(Stmt* a, Stmt* b);

  /* Default constructor. The next statement will be set to NullStmt. */
  Stmt ();

  // This constructor sets the next statement manually.
  Stmt (Stmt* nextStmt) {
    if (nextStmt != nullptr) children.push_back(nextStmt);
    next = nextStmt;
  }

  // Getter and setter for the next statement in sequence.
  Stmt* getNext() { return next; }
  void setNext(Stmt* nextStmt) {
    children.back() = nextStmt;
    next = nextStmt;
  }

  // This should only be false in the NullStmt class.
  bool hasNext() { return next != nullptr; }

  /* This is the command that must be implemented everywhere to
   * write the LLVM code to execute this Stmt.
   */
  virtual void exec(ostream& out, Context* con) {
    errout << "ERROR: exec not yet implemented in class " << typeid(*this).name() << endl;
    exit(3);
  }
};

/* This class is necessary to terminate a sequence of statements. */
class NullStmt :public Stmt {
public:
  NullStmt() :Stmt(nullptr) {
    nodeLabel = "Stmt:Null";
  }

  // Nothing to execute!
  void exec(ostream& out, Context* con) override { }
};

/* This is a statement for a block of code, i.e., code enclosed
 * in curly braces { and }.
 * Eventually, this is where scopes will begin and end.
 */
class Block :public Stmt {
private:
  Stmt* body;

public:
  Block(Stmt* b) {
    nodeLabel = "Stmt:Block";
    body = b;
    ASTchild(body);
  }
};

/* This class is for "if" AND "ifelse" statements. */
class IfStmt :public Stmt {
private:
  Exp* clause;
  Stmt* ifblock;
  Stmt* elseblock;

public:
  IfStmt(Exp* e, Stmt* ib, Stmt* eb) {
    nodeLabel = "Stmt:If";
    clause = e;
    ifblock = ib;
    elseblock = eb;
    ASTchild(clause);
    ASTchild(ifblock);
    ASTchild(elseblock);
  }
};

/* Class for while statements. */
class WhileStmt :public Stmt {
private:
  Exp* clause;
  Stmt* body;

public:
  WhileStmt(Exp* c, Stmt* b) {
    nodeLabel = "Stmt:While";
    clause = c;
    body = b;
    ASTchild(clause);
    ASTchild(body);
  }
};

/* A "new" statement creates a new binding of the variable to the
 * stated value.  */
class NewStmt :public Stmt {
private:
  Id* lhs;
  Exp* rhs;

public:
  NewStmt(Id* l, Exp* r) {
    nodeLabel = "Stmt:New";
    lhs = l;
    rhs = r;
    ASTchild(lhs);
    ASTchild(rhs);
  }
};

/* An assignment statement. This represents a RE-binding in the symbol table. */
class Asn :public Stmt {
private:
  Id* lhs;
  Exp* rhs;

public:
  Asn(Id* l, Exp* r) {
    nodeLabel = "Stmt:Asn";
    lhs = l;
    rhs = r;
    ASTchild(lhs);
    ASTchild(rhs);
  }
};

/* A single expression as a statement. */
class ExpStmt :public Stmt {
  private:
    Exp* body;

  public:
    ExpStmt(Exp* b) {
      body = b;
      ASTchild(body);
    }
};

/* A write statement. */
class Write :public Stmt {
private:
  Exp* val;

public:
  Write(Exp* v) {
    nodeLabel = "Stmt:Write";
    val = v;
    ASTchild(val);
  }

  void exec(ostream& out, Context* con) override; // implemented in ast.cpp
};

/* A lambda expression consists of a parameter name and a body. */
class Lambda :public Exp {
private:
  Id* var;
  Stmt* body;

protected:
  void writeLabel(ostream& out) { out << "lambda:exp" << flush; }

public:
  Lambda(Id* v, Stmt* b) {
    nodeLabel = "Exp:Lambda";
    var = v;
    body = b;
    ASTchild(var);
    ASTchild(body);
  }

  // These getter methods are necessary to support actually calling
  // the lambda sometime after it gets created.
  string& getVar() { return var->getVal(); }
  Stmt* getBody() { return body; }
};

/* A function call consists of the function name, and the actual argument.
 * Note that all functions are unary. */
class Funcall :public Exp {
private:
  Exp* funexp;
  Exp* arg;

public:
  Funcall(Exp* f, Exp* a) {
    nodeLabel = "Exp:Funcall";
    funexp = f;
    arg = a;
    ASTchild(funexp);
    ASTchild(arg);
  }
};

#endif //AST_HPP
