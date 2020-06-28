#include <map>
#include <stack>
#include <iostream>
#include <string>

using namespace std;

int main(){
  map<int,stack<string>> m;
  stack<string> s;
  m[0] = s;
  m[0].push("hello");
  m[0].push("hi");
  cout<<m[0].top()<<endl;
  return 0;
}
