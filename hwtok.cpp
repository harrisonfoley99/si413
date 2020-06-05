#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <cstdlib> //-- I need this for atoi
using namespace std;

/*** globals and prototypes *************************/
const int NUM=1, OPA=2, OPM=3, LP=4, RP=5, STOP=6;
const char* name[] = { "ERR", "NUM", "OPA", "OPM", "LP", "RP", "STOP" };

// this allows us a single type that can be either an int or a char
union SemVal {
  int val; 
  char sym;
};
union SemVal yylval;

int yylex();

/*** main ******************************************/
int main()
{
  int tok;
  while((tok = yylex()) && cin)
  {
    cout << name[tok] << '[';
    if (tok == NUM)
      cout << yylval.val << ']' << ' ';
    else if (tok == STOP)
      cout << ";]" << endl;
    else
      cout << yylval.sym << ']' << ' ';
  }
  return 0;
}

/*** binary to int ********************************/
// Input:  val - a string consisting of the form (0|1)+b , e.g. 1011b
// Output: the integer value equal to val (note: no error checking is done!)
int binary2int(const string &val)
{
  int x = 0;
  for(int i = 0; val[i] != 'b'; ++i)
    x = 2*x + (val[i]-'0');
  return x;
}

/*** scanner ***************************************/
int yylex()
{
  bool found = false;
  int state = 0;
  string val = "";
  while(!found)
  {
    char c = cin.get();
    switch(state)
    {
    case 0:
      switch(c) {
      case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        val += c; state = 1; break;
      case '1': case '0':
        val += c; state = 7; break;
      case '+': case '-': val += c; state = 2; break;
      case '*': case '/': val += c; state = 3; break;
      case ';': val += c; state = 4; break;
      case '(': val += c; state = 5; break;
      case ')': val += c; state = 6; break;
      case ' ': case '\t': case '\n': break;
      case EOF: found = true; break;
      default: cerr << "Bad char " << c << endl; break; }
      break;
    case 1:
      switch(c) {
      case '0':case '1':case '2':case '3':case '4':
      case '5':case '6':case '7':case '8': case '9':
        val += c; state = 1; break;
      default:
        cin.putback(c);
        found = true; }
      break;
    case 7:
      switch(c){
        case '0': case '1':
          val += c; state = 7; break;
        case 'b':
          val += c; state = 8; break;
        case '2': case '3': case '4': case '5': 
        case '6': case '7': case '8': case '9':
          val += c; state = 1; break; 
        default:
          cin.putback(c);
          found = true;}
      break;
    case 2: case 3: case 4: case 5: case 6: case 8:
      cin.putback(c);
      found = true;
      break;
    }
  }

  /* NOTE: However you add binary numbers, you'll want
           to set yyval.val to the integer value of that
           binary number.  And Remember: it's still a 
           NUM token! */
  //cout << endl << state << endl << val << endl;

  switch(state)
  { 
    case 0: return STOP;  // EOF
    case 1: yylval.val = atoi(val.c_str()); return NUM;
    case 2: yylval.sym = val[0];            return OPA;
    case 3: yylval.sym = val[0];            return OPM;
    case 4: yylval.sym = ';';               return STOP;
    case 5: yylval.sym = '(';               return LP;
    case 6: yylval.sym = ')';               return RP;
    case 7: yylval.val = atoi(val.c_str()); return NUM;
    case 8: yylval.val = binary2int(val.c_str()); return NUM;
  }

  //cout << endl;
}

