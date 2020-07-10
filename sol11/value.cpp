/* SI 413 Summer 2020
 * Lab 11
 * Implementation file for the Value class.
 */
#include "value.hpp"

void Value::writeTo(ostream& out) {
  switch(type) {
  case NUM_T: out << val.num; break;
  case BOOL_T: out << (val.tf ? "true" : "false"); break;
  case FUN_T: out << "closure"; break;
  case NONE_T: out << "UNSET"; break;
  }
}

bool Value::operator==(const Value& other) {
  switch(type) {
  case NUM_T: return val.num == other.val.num;
  case BOOL_T: return val.tf == other.val.tf;
  case FUN_T: return val.func.lamPtr == other.val.func.lamPtr && val.func.envPtr == other.val.func.envPtr;
  case NONE_T: return true;
  }
  return false;
}

// Returns pointer to first Frame containing reference to "name",
// NULL if none found
Frame* findFrame(const string &name, Frame* env)
{
  return env == nullptr || env->count(name) > 0 ? env : findFrame(name,env->getParent());
}

vector<Frame*> Frame::liveFrames;
