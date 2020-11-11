#ifndef COMPILATION_ENGINE_HPP
#define COMPILATION_ENGINE_HPP

#include <iostream>
#include <queue>
#include <regex>
#include <fstream>
#include "common.hpp"
#include "symbol_table.hpp"
#include "vm_writer.hpp"
using namespace std;
typedef pair<string, TypeTerminalSymbol> P;

class CompilationEngine: public VMWriter, public SymbolTable {
  private:
    queue<P> &que;
    void error(string err);
    string popSymbol();
    string popKeyword();
    string popIntConst();
    string popIdentifier();
    string popSringConst();
    string popType();
    string currentToken();
    bool checkInitialToken(string name);
    bool checkEndTermToken();
  public:
    CompilationEngine(queue<P>& terminalSymbolQue, string fileName):
    que(terminalSymbolQue), VMWriter(fileName){}
    void compileClass(); // name = "class"
    void compileClassVarDec(); // name = "classVarDec";
    void compileSubroutineDec(); // name = "subroutineDec";
    void compileSubroutineBody(string subroutineName, string subroutineType); // name="subroutineBody";
    void compileParameterList(); // name="parameterList";
    int compileVarDec(); // name = "varDec";
    void compileStatements(); // name = "statements";
    void compileDo(); // name ="doStatement";
    void compileLet(); // name = "letStatement";
    void compileWhile(); // name = "whileStatement";
    void compileReturn(); // name = "returnStatement";
    void compileIf(); // name = "ifStatement";
    void compileTerm();  // name = "term";
    void compileExpression(); // name = "expression";
    int compileExpressionList(); // name = "expressionList";
};

inline string CompilationEngine::currentToken() {
  if (!que.empty()) return que.front().first;
  cerr << "que is empty" << "\n";
  return "";
}

inline void CompilationEngine::error(string currentFunctionName) {
  cerr << "========== compile error ========= "<< "\n";
  cerr << currentToken() << "\n";
  cerr << currentFunctionName << "\n";
}

inline bool CompilationEngine::checkEndTermToken() {
  string token = currentToken();
  return (token == "+" || token == "-" || token == "*"
        || token == "/" || token == "&amp;" || token == "|"
        || token == "&lt;" || token == "&gt;" || token == "=");
}

#endif // COMPILATION_ENGINE_HPP