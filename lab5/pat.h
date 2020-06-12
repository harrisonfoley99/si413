/*** This is file: pat.h ***/
#include <string>
#include <vector>
using namespace std;

// Token labels
const int ATOM=1, FOLD=2, STOP=3, COLON=4, 
          NAME=5, POP=6, LB=7, RB=8;

// Prototype for yylex(), aka getNextToken()
int yylex();

// declaration for global containing the
// semantic value of the token from yylex
extern string yylval;

// Helper functions
vector<string> fold(const vector<string> &A, const vector<string> &B);
vector<string> concat(const vector<string> &A, const vector<string> &B);
vector<string> rev(const vector<string> &A);
string getTokenName(int tok);
