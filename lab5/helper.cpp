#include "pat.h"
#include <iostream>
using namespace std;

vector<string> fold(const vector<string> &A, const vector<string> &B)
{
  vector<string> res;
  for(unsigned int i = 0; i < A.size() || i < B.size(); ++i)
  { res.push_back(A[min((int)i,(int)A.size()-1)]);
    res.push_back(B[min((int)i,(int)B.size()-1)]); }
  return res;
}
vector<string> concat(const vector<string> &A, const vector<string> &B)
{
  vector<string> res;
  for(int i = 0; i < A.size(); ++i) res.push_back(A[i]);
  for(int i = 0; i < B.size(); ++i) res.push_back(B[i]);
  return res;
}
vector<string> rev(const vector<string> &A)
{
  vector<string> res;
  for(int i = A.size() - 1; i >= 0; --i) res.push_back(A[i]);
  return res;
}

string getTokenName(int tok)
{
  const string name[] =
    {"EOF","ATOM","FOLD","STOP","COLON","NAME","POP","LB","RB"};
  //   0     1      2       3      4       5     6    7    8
  if (tok < 0 || tok > 8) {
    cerr << "No token with id " << tok << endl;
    exit(1);
  }
  return name[tok];
}

