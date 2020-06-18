/*** This is file: pat.h ***/
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Prototype for yylex(), aka getNextToken()
int yylex();

// Prints out a vector of strings with spaces in between
// You can call this just like resout << some_vector << endl;
ostream& operator<< (ostream& out, const vector<string>* vec);

// Computes the "fold" or interleaving of two vectors of strings
vector<string>* fold(const vector<string> *A, const vector<string> *B);

// Concatenates two vectors of strings
vector<string>* concat(const vector<string> *A, const vector<string> *B);

// Reverses a vector of strings
vector<string>* rev(const vector<string> *A);
