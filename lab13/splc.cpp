/* SI413 Fall 2018
 * Lab 13
 * Main function to run the compiler
 * Harrison Foley, 211926
 */

#include <fstream>
#include <vector>
#include "splc.hpp"
#include "spl.tab.hpp"
#include "ast.hpp"
#include "readlineistream.hpp"

// global variable declarations
colorout resout(1, 'u');
colorout errout(2, 'r');
stringstream delayout;

//Use of another string stream discussed with Byron Gallager (no code was exchanged)
stringstream global_declarations;
stringstream main_stream;

Stmt* tree = nullptr;
extern std::istream *lexer_istream;

int main(int argc, char** argv) {
  // 0, 1, and 2 correspond to stdin, stdout, and stderr respectively.
  bool showPrompt = isatty(0);
  bool interactive = showPrompt && isatty(1);  

  // this saves the entire program's AST to deallocate at the end
  vector<Stmt*> program;

  // figure out filenames if there is a command-line argument for that
  if (argc >= 2) {
    lexer_istream = new ifstream(argv[1]);
    if (!*lexer_istream) {
      cerr << "Could not open input file \"" << argv[1] << "\"!" << endl;
      exit(2);
    }
    string ofname = argv[1];
    int dotind = ofname.rfind('.');
    if (dotind != string::npos) {
      ofname = ofname.substr(0, dotind);
    }
    ofname += ".ll";
    if (!resout.redirect(ofname)) {
      cerr << "Could not open output file \"" << ofname << "\"" << endl;
      exit(2);
    }
    interactive = false;
    cerr << "Reading input from " << argv[1] << " and writing output to " << ofname << endl;
  }
  else if (interactive) {
    readlineIstream* p = new readlineIstream(0);
    p->setPrompt("spl> ");    
    lexer_istream = p; 
  }
  else
    lexer_istream = &cin;

  // LLVM header stuff
  global_declarations << "target triple = \"x86_64-pc-linux-gnu\"" << endl
         << "@pfmt0 = constant [6 x i8] c\"spl> \\00\"" << endl
         << "@pfmt = constant [5 x i8] c\"%ld\\0A\\00\"" << endl
         << "declare i32 @printf(i8*,...)" << endl
         << "declare i32 @scanf(i8*,...)" << endl;

  // this is the global context, used to keep track of any contextual
  // information you need as you traverse the AST. (For example the
  // counter to get the next register/llvm-variable name.
  Context gcon;
  map<string,string> global_scope;
  gcon.scopeStack.push_back(global_scope);

  // start LLVM main
  main_stream << "define i32 @main() {" << endl;

  // loop to read in program statements, one at a time
  tree = nullptr;
  while(true) {
    tree = nullptr;
    int pres = yyparse();
    if (interactive) { /* INTERACTIVE */
      if (tree == nullptr) { cout << "Goodbye" << endl; break; }
    }
    else { /* NON-INTERACTIVE */
      if (pres != 0 || tree == nullptr) break;
    }

    if (tree) {
      program.push_back(tree);
      tree->exec(main_stream,&gcon);
    }
  }

  // end LLVM main
  main_stream << "    ret i32 0" << endl
         << "}" << endl;

  // write all accumulated streams in order
  resout << global_declarations.str();
  resout << main_stream.str();
  resout << delayout.str();
  
  // cleanup
  for (Stmt* node : program) delete node;
  yylex_destroy();
  if (lexer_istream != &cin) delete lexer_istream;

  return 0;
}
