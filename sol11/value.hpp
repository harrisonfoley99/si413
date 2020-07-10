/* SI 413 Summer 2020
 * MIDN Harrison Foley
 * Lab 11
 * C++ header file for the Value class.
 */

#ifndef VALUE_HPP
#define VALUE_HPP

#include <vector>
#include <iostream>
#include <cstdlib>
#include <map>
#include <stack>
using namespace std;

int recerr(string msg);

// Forward declaration of the Lambda AST class so we can have
// pointers to Lambda objects.
class Lambda;
class Frame;

// Closure class!
struct Closure {
  Lambda* lamPtr;
  Frame* envPtr;
};

// This gives the type of what's stored in the Value object.
// NONE_T means nothing has been set yet.
enum VType { NUM_T=0, BOOL_T, FUN_T, NONE_T };
inline string tname(VType t)
{
  static string name[4] = { "NUM", "BOOL", "FUN", "UNSET" };
  return name[t];
}

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

   int num() {
    return getType() == NUM_T ? val.num :
      recerr("Type mismatch: expected NUM, got " + tname(getType()));
  }

  bool tf() {
    return getType() == BOOL_T ? val.tf :
      recerr("Type mismatch: expected BOOL, got " + tname(getType()));
  }

  Closure func() {
    return getType() == FUN_T ? val.func :
      (recerr("Type mismatch: expected FUN, got " + tname(getType())), Closure());
  }

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

// Frames!
class Frame : public map<string,Value>
{
private:
  Frame* parent;
  static vector<Frame*> liveFrames;
public:
  static void cleanup() {
    for(Frame* f : liveFrames) delete f;
  }
  Frame(Frame* par) { 
    parent = par; 
    //cerr << "Frame " << this << " is born!" << endl; 
    liveFrames.push_back(this);
  }
  Frame* getParent() { return parent; }

  //~Frame() { cerr << "Frame " << this << " has died!" << endl; }
};
// Returns pointer to first Frame containing reference to "name",
// NULL if none found
Frame* findFrame(const string &name, Frame* env);

#endif // VALUE_HPP
