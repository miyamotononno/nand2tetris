#ifndef COMPILATION_ENGINE_HPP
#define COMPILATION_ENGINE_HPP

#include <string>
#include <iostream>
#include <queue>
#include <regex>
#include <fstream>
#include "constants.hpp"
using namespace std;
typedef pair<string, TypeTerminalSymbol> P;

class BaseCompilation {
  public:
    string name;
  protected:
    ofstream *ofs;
    queue<P> &que;
    int indent;
    void error(string currentFunctionName="");
    void writeSymbol();
    void writeKeyword();
    void writeIntConst();
    void writeIdentifier();
    void writeSringConst();
    void writeType();
    string currentToken();
    void writeSubroutineCall(bool initalToken=true); // subroutine callに関してはタグをつけないのでここで定義
  public:
    BaseCompilation(string className, queue<P>& terminalSymbolQue, ofstream *output_file, int _indent)
    :que(terminalSymbolQue){
      ofs = output_file;
      name = className;
      indent = _indent;
      for (int i=0; i<2*_indent; i++) *ofs << ' ';
      *ofs << '<' << className << '>' << "\n";
      indent++;
    }
    ~BaseCompilation();
};

inline string BaseCompilation::currentToken() {
  if (!que.empty()) return que.front().first;
  cerr << "que is empty" << "\n";
  return "";
}

inline void BaseCompilation::error(string currentFunctionName) {
  cerr << "========== compile error ========= "<< "\n";
  cerr << name << "\n";
  cerr << currentToken() << "\n";
  cerr << currentFunctionName << "\n";
}

inline BaseCompilation::~BaseCompilation() {
  indent--;
  for (int i=0; i<2*indent; i++) *ofs << ' ';
  *ofs << "</" << name << '>' << "\n";
}

class TokenClass: public BaseCompilation {
  ofstream *aa;
  public:
    TokenClass(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return token == "class";
    }
};

class TokenClassVarDec: public BaseCompilation {
  public:
    TokenClassVarDec(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return token == "static" || token == "field";
    }
};

class TokenSubroutine: public BaseCompilation {
  public:
    TokenSubroutine(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return (token == "constructor" || token == "function" || token == "method" || token == "void");
    }
};

class TokenSubroutineBody: public BaseCompilation {
  public:
    TokenSubroutineBody(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
};

class TokenParameterList: public BaseCompilation {
  public:
    TokenParameterList(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return regex_match(token, regex(RegexPattern::identifier));
    }
};

class TokenVarDec: public BaseCompilation {
  public:
    TokenVarDec(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return token == "var";
    }
};

class TokenStatements: public BaseCompilation {
  public:
    TokenStatements(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return (token == "let" || token == "if" || token == "while"
           || token == "do" || token == "return");
    }
};

class TokenDo: public BaseCompilation {
  public:
    TokenDo(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return token == "do";
    }
};

class TokenLet: public BaseCompilation {
  public:
    TokenLet(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return token == "let";
    }
};

class TokenWhile: public BaseCompilation {
  public:
    TokenWhile(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return token == "while";
    }
};

class TokenReturn: public BaseCompilation {
  public:
    TokenReturn(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return token == "return";
    }
};

class TokenIf: public BaseCompilation {
  public:
    TokenIf(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(string token) {
      return token == "if";
    }
};

class TokenTerm: public BaseCompilation {
  public:
    TokenTerm(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(P p) { // ToDo: change!
      if (p.second == TypeTerminalSymbol::IntConst) return true;
      if (p.second == TypeTerminalSymbol::StringConst) return true;
      if (p.second == TypeTerminalSymbol::Keyword) return true;
      if (p.second == TypeTerminalSymbol::Identifier) return true;
      if (p.first == "(" || p.first == "-" || p.first == "~") return true;
      return false;
    }
    static bool checkEndToken(string token) {
      return (token == "+" || token == "-" || token == "*"
           || token == "/" || token == "&amp;" || token == "|"
           || token == "&lt;" || token == "&gt;" || token == "=");
    } 
};

class TokenExpression: public BaseCompilation {
  public:
    TokenExpression(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent):
    BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(P p) {
      return TokenTerm::checkInitialToken(p);
    }
};

class TokenExpressionList: public BaseCompilation {
  public:
    TokenExpressionList(string name, queue<P>& terminalSymbolQue, ofstream *output_file, int indent)
    :BaseCompilation(name, terminalSymbolQue, output_file, indent){}
    void compile();
    static bool checkInitialToken(P p) {
      return TokenExpression::checkInitialToken(p);
    }
};

class CompilationEngine {
  private:
    string name;
    queue<P> &que;
    ofstream *ofs;
    int indent;
  public:
    CompilationEngine(queue<P>& terminalSymbolQue, ofstream *output_file, int num):
    que(terminalSymbolQue), ofs(output_file), indent(num){}
    void compileClass() {
      name = "class";
      TokenClass tc(name, que, ofs, indent);
      tc.compile();
    }
    void compileClassVarDec() {
      name = "classVarDec";
      TokenClassVarDec tcvd(name, que, ofs, indent);
      tcvd.compile();
    }
    void compileSubroutine() {
      name = "subroutineDec";
      TokenSubroutine tst(name, que, ofs, indent);
      tst.compile();
    }
    void compileSubroutineBody() {
      name="subroutineBody";
      TokenSubroutineBody tsb(name, que, ofs, indent);
      tsb.compile();
    }
    void compileParameterList() {
      name="parameterList";
      TokenParameterList cpl(name, que, ofs, indent);
      cpl.compile();
    }
    void compileVarDec() {
      name = "varDec";
      TokenVarDec cvd(name, que, ofs, indent);
      cvd.compile();
    }
    void compileStatements() {
      name = "statements";
      TokenStatements tsm(name, que, ofs, indent);
      tsm.compile();
    }
    void compileDo() {
      name ="doStatement";
      TokenDo cd(name, que, ofs, indent);
      cd.compile();
    }
    void compileLet() {
      name = "letStatement";
      TokenLet cl(name, que, ofs, indent);
      cl.compile();
    }
    void compileWhile() {
      name = "whileStatement";
      TokenWhile cw(name, que, ofs, indent);
      cw.compile();
    }
    void compileReturn() {
      name = "returnStatement";
      TokenReturn cr(name, que, ofs, indent);
      cr.compile();
    }
    void compileIf() {
      name = "ifStatement";
      TokenIf ci(name, que, ofs, indent);
      ci.compile();
    }
    void compileExpression() {
      name = "expression";
      TokenExpression ce(name, que, ofs, indent);
      ce.compile();
    }
    void compileTerm() {
      name = "term";
      TokenTerm ct(name, que, ofs, indent);
      ct.compile();
    }
    void compileExpressionList() {
      name = "expressionList";
      TokenExpressionList cel(name, que, ofs, indent);
      cel.compile();
    }
};

#endif // COMPILATION_ENGINE_HPP