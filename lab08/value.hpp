/* SI 413 Summer 2020
 * Lab 8
 * C++ header file for the Value class.
 * Foley 211926
 */

#ifndef VALUE_HPP
#define VALUE_HPP

#include <iostream>
#include <cstdlib>
#include <map>
#include <stack>

using namespace std;

// Forward declaration of the Lambda AST class so we can have
// pointers to Lambda objects.
class Lambda;
class Frame;

struct Closure {
  Lambda* lamPtr;
  Frame* envPtr;
};
// This gives the type of what's stored in the Value object.
// NONE_T means nothing has been set yet.  The function tname
// translates a type to a string with that type's name.
enum VType { NUM_T, BOOL_T, FUN_T, NONE_T };
inline string tname(VType t)
{
  static string name[4] = { "NUM", "BOOL", "FUN", "UNSET" };
  return name[t];
}

// This is the actual class Value.
class Value {
private:
  // The value is either an int, a bool, or a pointer to a Lambda.
  union { int num; bool tf; Closure func; } val;
  VType type;

public:
  /* default constructor, set to NONE TYPE */
  explicit Value() { type = NONE_T; }

  /* set the type to NUM */
  explicit Value(int n) { type = NUM_T; val.num = n; }

  /* set the type to BOOL */
  explicit Value(bool b) { type = BOOL_T; val.tf = b; }

  /* set the type to FUN */
  explicit Value(Closure ptr) { type = FUN_T; val.func = ptr; }

  VType getType() { return type; }
  void setType(VType t) { type = t; }

  int num() { return val.num; }

  bool tf() { return val.tf; }

  Closure func() { return val.func; }

  /* Writes a representation of this Value object to the
   * named output stream, according to the stored type.
   */
  void writeTo(ostream& out);

  /* This function makes equality testing work using the == operator.
   * So for instance if we have Value v1, v2; we can write v1 == v2
   * and get true back only if they have the same type and value.
   * Note that != is NOT implemented.
   */
  bool operator==(const Value& other);
};

// forward declaration of the global variable representing a symbol table.
//extern std::map<string,stack<Value>> symTab;
class Frame : public map<string,Value>
{
private:
  Frame* parent;
public:
  Frame(Frame* par) { parent = par; }
  Frame* getParent() { return parent; }
};

#endif // VALUE_HPP
