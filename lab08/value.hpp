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
#include <vector>
#include <algorithm>

using namespace std;
#include "colorout.hpp"

// Forward declaration of the Lambda AST class so we can have
// pointers to Lambda objects.
class Lambda;
class Frame;

// Declare the output streams to use everywhere
extern colorout resout;
extern colorout errout;

// Global variable to indicate if an error has occurred.
extern bool error;

struct Closure {
  Lambda* lamPtr;
  Frame* envPtr;
};
// This gives the type of what's stored in the Value object.
// NONE_T means nothing has been set yet.  The function tname
// translates a type to a string with that type's name.
enum VType { NUM_T = 0, BOOL_T, FUN_T, NONE_T };
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

  int num() {
    if(tname(type) != "NUM") {
      error = true;
      errout << "Type mismatch: expected NUM, got " << tname(type) << endl;
    }
    return val.num; 
  }

  bool tf() { 
    if(tname(type) != "BOOL") {
      error = true;
      errout << "Type mismatch: expected BOOL, got " << tname(type) << endl;
    }
    return val.tf; 
  }

  Closure func() { 
    if(tname(type) != "FUN") {
      error = true;
      errout << "Type mismatch: expected FUN, got " << tname(type) << endl;
    }
    return val.func; 
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

// forward declaration of the global variable representing a symbol table.
//extern std::map<string,stack<Value>> symTab;
class Frame : public map<string,Value>
{
  private:
    static vector<Frame*> liveFrames;
    Frame* parent;
    bool marked;
  public:
    static void cleanup() { 
      for (Frame* f : liveFrames) delete f;
    }

    static void markedFrames(){
      for (Frame* f : liveFrames){
        if(f->marked) cout << f << endl;
      }
    }

    Frame(Frame* par) { 
      parent = par;
      marked = false; 
      //cerr << "Frame " << this << " is born!" << endl; 
      liveFrames.push_back(this);
    }

    void unmark(){
      if(marked) marked = false;
    }

    static void mark(Frame* env){
      if(env != nullptr && env->marked == true){
        return;
      }
      env->marked = true;
      if(env->getParent() != nullptr) {
        mark(env->getParent());
      }
      for(map<string,Value>::iterator itr = env->begin(); itr != env->end(); ++itr){
        if(tname(itr->second.getType()) == "FUN"){
          mark(itr->second.func().envPtr);
        }
      }
    }


    //Worked with Tom Dias, though no code was shared and all eyes on screen and hands
    //on keyboard were our own. Specifically we agreed on using a for loop and writing our
    //own swap alorithms. We did not talk about how to write a swap algorithm, though we
    //did agree on the name "JaNk SwAp" 
    static void sweep(){
      for(int i = 0; i < liveFrames.size(); i++){
        //delete all live frames unmarked
        if((liveFrames[i]->marked) == false){
          
          Frame* tmp = liveFrames[i];
          liveFrames[i] = liveFrames[liveFrames.size() - 1];
          liveFrames[liveFrames.size() - 1] = tmp;
          
          delete liveFrames.back();
          liveFrames.pop_back();
          i--;
        }
      }
      //unmark everything
      for (Frame* f : liveFrames) f->unmark();
    }

    static int getNumFrames() { return liveFrames.size(); }
    //~Frame() { cerr << "Frame " << this << " has died!" << endl; }
    Frame* getParent() { return parent; }
};

#endif // VALUE_HPP
