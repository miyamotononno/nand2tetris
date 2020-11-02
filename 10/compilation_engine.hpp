#ifndef COMPILATION_ENGINE_HPP
#define COMPILATION_ENGINE_HPP

#include <string>
#include <fstream>
using namespace std;

class CompilationEngine {
  private:
  public:
    CompilationEngine(ifstream *inputFile, ofstream *outputFile);
    void complieClass();
    void complieClassVarDec();
    void complieSubroutine();
    void complieParameterList();
    void complieVarDec();
    void complieStatements();
    void complieDo();
    void complieLet();
    void complieWhile();
    void complieReturn();
    void complieIf();
    void complieExpression();
    void complieTerm();
    void complieExpressionList();
};

#endif // COMPILATION_ENGINE_HPP