/* SI 413 Summer 2020
 * Lab 8
 * Implementation file for the Value class.
 * Foley 211926
 */
#include "value.hpp"

void Value::writeTo(ostream& out) {
  switch(type) {
  case NUM_T: out << val.num; break;
  case BOOL_T: out << (val.tf ? "true" : "false"); break;
  case FUN_T: out << "lambda expression"; break;
  case NONE_T: out << "UNSET"; break;
  }
}

bool Value::operator==(const Value& other) {
  switch(type) {
  case NUM_T: return val.num == other.val.num;
  case BOOL_T: return val.tf == other.val.tf;
  case FUN_T: return val.func == other.val.func;
  case NONE_T: return true;
  }
  return false;
}

// This is the actual definition of the symbol table.
std::map<string,Value> symTab;

