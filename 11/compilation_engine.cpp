#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include "compilation_engine.hpp"
using namespace std;

string CompilationEngine::popSymbol() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::Symbol) {
    que.pop();
    return p.first;
  } else error("popSymbol");
  return "";
}

string CompilationEngine::popKeyword() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::Keyword) {
    que.pop();
    return p.first;
  } else error("popKeyword");
  return "";
}

string CompilationEngine::popIdentifier() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::Identifier) {
    que.pop();
    return p.first;
  } else error("popIdentifier");
  return "";
}

string CompilationEngine::popIntConst() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::IntConst) {
    que.pop();
    return p.first;
  } else error("popIntConst");
  return "";
}

string CompilationEngine::popSringConst() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::StringConst) {
    que.pop();
    return p.first;
  } else error("popSringConst");
  return "";
}

string CompilationEngine::popType() {
  P p = que.front();
  string word;
  if (regex_match(p.first, regex(RegexPattern::type))) {
    if (p.second == TypeTerminalSymbol::Keyword) word = popKeyword();
    else word = popIdentifier();
  } else error("popType");
  return word;
}

bool CompilationEngine::checkInitialToken(string name) {
  string token = currentToken();
  if (name == "classVarDec") {
    return token == "static" || token == "field";
  }
  if (name == "subroutineDec") {
    return (token == "constructor" || token == "function" || token == "method" || token == "void");
  }
  if (name == "varDec") return token == "var";
  if (name == "parameterList") return regex_match(token, regex(RegexPattern::identifier));
  if (name == "doStatement") return token == "do";
  if (name == "letStatement") return token == "let";
  if (name == "whileStatement") return token == "while";
  if (name == "returnStatement") return token == "return";
  if (name == "ifStatement") return token == "if";
  
  // 以下, term, expression, 
  P pairToken = que.front();
  if (pairToken.second == TypeTerminalSymbol::IntConst) return true;
  if (pairToken.second == TypeTerminalSymbol::StringConst) return true;
  if (pairToken.second == TypeTerminalSymbol::Keyword) return true;
  if (pairToken.second == TypeTerminalSymbol::Identifier) return true;
  if (token == "(" || token == "-" || token == "~") return true;
  return false;
}

void CompilationEngine::compileClass(){
  popKeyword(); // "class"
  currentClassName = popIdentifier(); // className
  popSymbol(); // {
  while (checkInitialToken("classVarDec")) {
    compileClassVarDec();
  }
  while (checkInitialToken("subroutineDec")) {
    compileSubroutineDec();
  }
  popSymbol(); // }
}

void CompilationEngine::compileClassVarDec() {
  string keyword = popKeyword();
  IdentifierAttribute kind;
  if (keyword == "static") kind = IdentifierAttribute::Static;
  else if (keyword == "field") kind = IdentifierAttribute::Field;
  else error("class var dec");
  string type = popType();
  string name = popIdentifier();
  define(name, type, kind);
  while(true) {
    if (currentToken() == ",") {
      popSymbol();
      name = popIdentifier();
      define(name, type, kind);
    } else if (currentToken() == ";") {
      popSymbol();
      break;
    } else {
      error("class var dec");
      break;
    }
  }
}

void CompilationEngine::compileSubroutineDec() {
  startSubroutine();
  string subroutineType = popKeyword(); // constructor, function or method
  if (currentToken() == "void") popKeyword();
  else popType(); // type
  string subroutineName = popIdentifier(); // subroutiename
  popSymbol(); // (
  
  compileParameterList();
  if (subroutineType=="method") {
    define("this", currentClassName, IdentifierAttribute::Arg);
  }
  popSymbol(); // )
  compileSubroutineBody(subroutineName, subroutineType);
}

void CompilationEngine::compileSubroutineBody(string subroutineName, string subroutineType) {
  popSymbol(); // {
  int nLocals = 0;
  while (checkInitialToken("varDec")) {
    nLocals+=compileVarDec();
  }
  string functionName = currentClassName + '.' + subroutineName;
  writeFunction(functionName, nLocals);
  if (subroutineType=="method") {
    writePush(Segment::Arg, 0);
    writePop(Segment::Pointer, 0);
  } else if (subroutineType == "constructor") {
    writePush(Segment::Const, varCount(IdentifierAttribute::Field));
    writeCall("Memory.alloc", 1);
    writePop(Segment::Pointer, 0);
  }
  compileStatements();
  popSymbol();
}

void CompilationEngine::compileParameterList() {
  if (!checkInitialToken("parameterList")) return;
  string type = popType();
  string name = popIdentifier();
  define(name, type, IdentifierAttribute::Arg);
  while(true) {
    if (currentToken() == ",") {
      popSymbol();
      type = popType();
      name = popIdentifier();
      define(name, type, IdentifierAttribute::Arg);
    } else break;
  }
}

int CompilationEngine::compileVarDec() {
  int varNum = 0;
  popKeyword(); // var
  string type = popType();
  string name = popIdentifier();
  define(name, type, IdentifierAttribute::Var);
  varNum++;
  while(currentToken() == ",") {
    popSymbol(); // ,
    name = popIdentifier(); // varName
    define(name, type, IdentifierAttribute::Var);
    varNum++;
  }
  popSymbol(); // ;
  return varNum;
}

void CompilationEngine::compileStatements() {
  string keyword;
  while(true) {
    keyword = currentToken();
    if (checkInitialToken("doStatement")) compileDo();
    else if (checkInitialToken("letStatement")) compileLet();
    else if (checkInitialToken("whileStatement")) compileWhile();
    else if (checkInitialToken("ifStatement")) compileIf();
    else if (checkInitialToken("returnStatement")) compileReturn();
    else break;
  }
}

void CompilationEngine::compileDo() {
  popKeyword(); // do

  // 以下subroutine callをしていることに注意
  string name = popIdentifier(); // 1. srName 2. srName or className
  IdentifierAttribute kind = kindOf(name);
  if (currentToken() == ".") { // srName or className
    name+=popSymbol(); // .
    name+=popIdentifier(); // srName
    if (kind == IdentifierAttribute::None) { // 他クラスのsubroutine
      popSymbol(); // (
      int nArg = compileExpressionList();
      popSymbol(); // )
      writeCall(name, nArg);
    } else { // そのクラスのfunction or constructor
      if (kind==IdentifierAttribute::Arg) {
        writePush(Segment::Arg, indexOf(name));
      } else if (kind == IdentifierAttribute::Field) {
        writePush(Segment::This, indexOf(name));
      } else if (kind == IdentifierAttribute::Static) {
        writePush(Segment::Static, indexOf(name));
      } else { // var
        writePush(Segment::Local, indexOf(name));
      }
      int nArg = compileExpressionList();
      popSymbol(); // )
      writeCall(name, nArg+1);
    }  
  } else { // そのクラスのmethod
    popSymbol(); // (
    writePush(Segment::Pointer, 0);
    int nArg = compileExpressionList();
    popSymbol(); // )
    name = currentClassName+'.'+name;
    writeCall(name, nArg+1);
  }

  popSymbol(); // ;
  writePop(Segment::Temp, 0);
}

void CompilationEngine::compileLet() {
  popKeyword(); // let
  string name = popIdentifier(); // varName
  IdentifierAttribute kind = kindOf(name); // Noneの場合はない
  int index = indexOf(name);
  bool flag = false;
  if (currentToken() != "=") { // 配列への代入
    popSymbol(); // [
    compileExpression();
    writeArithmetic(Command::Add);
    popSymbol(); // ]
    writePop(Segment::Pointer, 1);
    popSymbol(); // =
    if (kind == IdentifierAttribute::Arg) {
      writePush(Segment::Arg, index);
    } else if (kind == IdentifierAttribute::Field) {
      writePush(Segment::This, index);
    } else if (kind == IdentifierAttribute::Static) {
      writePush(Segment::Static, index);
    } else { // var
      writePush(Segment::Local, index);
    }
    writeArithmetic(Command::Add);
    writePop(Segment::Temp, 2);
    
    compileExpression();

    writePush(Segment::Temp, 2);
    writePop(Segment::Pointer, 1);
    writePop(Segment::That, 0);

    popSymbol(); // ;
  } else {
    popSymbol(); // =
    compileExpression();
    popSymbol(); // ;
    if (kind == IdentifierAttribute::Arg) {
      writePop(Segment::Arg, index);
    } else if (kind == IdentifierAttribute::Field) {
      writePop(Segment::This, index);
    } else if (kind == IdentifierAttribute::Static) {
      writePop(Segment::Static, index);
    } else { // var
      writePop(Segment::Local, index);
    } 
  }
}

void CompilationEngine::compileWhile() {
  popKeyword(); // while
  string whileLabel = getNewLabel();
  writeLabel(whileLabel);
  popSymbol(); // (
  compileExpression();
  popSymbol(); // )
  writeArithmetic(Command::Not); // falseの場合に抜け出すので
  string endLabel = getNewLabel();
  writeIf(endLabel);
  popSymbol(); // {
  compileStatements();
  writeGoto(whileLabel);
  popSymbol(); // }
  writeLabel(endLabel);
}

void CompilationEngine::compileIf(){
  popKeyword(); // if
  popSymbol();
  compileExpression(); // 条件式'
  writeArithmetic(Command::Not); // falseの場合にelseへ行くので
  string elseLabel = getNewLabel();
  writeIf(elseLabel);
  popSymbol();
  popSymbol();
  compileStatements(); // if文の中身
  popSymbol();
  string endLabel = getNewLabel();
  writeGoto(endLabel);
  if (currentToken() == "else") {
    writeLabel(elseLabel);
    popKeyword();
    popSymbol();
    compileStatements();
    popSymbol();
    writeLabel(endLabel);
  } else {
    writeLabel(elseLabel);
  }
}

void CompilationEngine::compileReturn() {
  popKeyword();
  if (currentToken() != ";") compileExpression();
  else writePush(Segment::Const, 0);
  writeReturn();
  popSymbol();
}

void CompilationEngine::compileTerm() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::IntConst) {
    string num = popIntConst();
    writePush(Segment::Const, stoi(num));
  }
  else if (p.second == TypeTerminalSymbol::StringConst) {
    string str = popSringConst();
    writePush(Segment::Const, str.size());
    writeCall("String.new", 1);
    for (auto c: str) {
      writePush(Segment::Const, int(c));
      writeCall("String.appendChar", 2);
    }
  }
  else if (p.second == TypeTerminalSymbol::Keyword) {
    string keyword = popKeyword(); // true, false, null, thisのどれか
    if (keyword=="true") {
      writePush(Segment::Const, 1);
      writeArithmetic(Command::Neg);
    } else if (keyword=="this") {
      writePush(Segment::This, 0);
    } else {
      writePush(Segment::Const, 0);
    }
  }
  else if (p.second == TypeTerminalSymbol::Identifier) {
    string name = popIdentifier();
    if (checkEndTermToken()) return; // varName
    if (currentToken() == "[") { // [ exp ]
      popSymbol(); // [
      compileExpression();
      popSymbol(); // ]
      writeArithmetic(Command::Add);
      writePop(Segment::Pointer, 1);
      writePush(Segment::That, 0);
    } else if (currentToken() == "(" || currentToken() == "."){
      IdentifierAttribute kind = kindOf(name);
      if (currentToken() == ".") { // srName or className
        name+=popSymbol(); // .
        name+=popIdentifier(); // srName
        if (kind == IdentifierAttribute::None) { // 他クラスのsubroutine
          popSymbol(); // (
          int nArg = compileExpressionList();
          popSymbol(); // )
          writeCall(name, nArg);
        } else { // そのクラスのfunction or constructor
          if (kind==IdentifierAttribute::Arg) {
            writePush(Segment::Arg, indexOf(name));
          } else if (kind == IdentifierAttribute::Field) {
            writePush(Segment::This, indexOf(name));
          } else if (kind == IdentifierAttribute::Static) {
            writePush(Segment::Static, indexOf(name));
          } else { // var
            writePush(Segment::Local, indexOf(name));
          }
          int nArg = compileExpressionList();
          popSymbol(); // )
          writeCall(name, nArg+1);
        }  
      } else { // そのクラスのmethod
        popSymbol(); // (
        writePush(Segment::Pointer, 0);
        int nArg = compileExpressionList();
        popSymbol(); // )
        name = currentClassName+'.'+name;
        writeCall(name, nArg+1);
      }
    }
  } else {
    if (p.first == "(") { // (expression)
      popSymbol();
      compileExpression();
      popSymbol();
    } else {
      string com = popSymbol();
      compileTerm();
      if (com == "-") writeArithmetic(Command::Neg);
      else writeArithmetic(Command::Not);
    } 
  }
}

void CompilationEngine::compileExpression() {
  compileTerm(); // pushはcompileTermの中で行う
  while(checkEndTermToken()) {
    string op = popSymbol();
    compileTerm();
    writeArithmetic(OperationMap[op]);
  }
}

int CompilationEngine::compileExpressionList() {
  int argNum=0;
  if (checkInitialToken("expressionList")) {
    compileExpression();
    argNum++;
    while(currentToken() == ",") {
      popSymbol();
      compileExpression();
      argNum++;
    }
  }
  return argNum;
}