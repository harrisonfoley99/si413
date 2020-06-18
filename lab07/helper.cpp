#include "pat.h"
using namespace std;

// Prints out a vector of strings with spaces in between
// You can call this just like resout << some_vector << endl;
ostream& operator<< (ostream& out, const vector<string>* vec) {
  if (vec->empty()) return out;
  out << (*vec)[0];
  for (unsigned long i=1; i<vec->size(); ++i)
    out << ' ' << (*vec)[i];
  return out;
}
// Computes the "fold" or interleaving of two vectors of strings
vector<string>* fold(const vector<string> *A, const vector<string> *B) {
  vector<string>* res = new vector<string>;
  unsigned long int i = 0;
  for (; i < A->size() && i < B->size(); ++i) {
    res->push_back((*A)[i]);
    res->push_back((*B)[i]);
  }
  for (; i < A->size(); ++i)
    res->push_back((*A)[i]);
  for (; i < B->size(); ++i)
    res->push_back((*B)[i]);
  return res;
}

// Concatenates two vectors of strings
vector<string>* concat(const vector<string> *A, const vector<string> *B) {
  vector<string>* res = new vector<string>;
  unsigned long int i;
  for(i = 0; i < A->size(); ++i) res->push_back((*A)[i]);
  for(i = 0; i < B->size(); ++i) res->push_back((*B)[i]);
  return res;
}

// Reverses a vector of strings
vector<string>* rev(const vector<string> *A) {
  vector<string>* res = new vector<string>;
  long int i;
  for(i = A->size() - 1; i >= 0; --i) res->push_back((*A)[i]);
  return res;
}
