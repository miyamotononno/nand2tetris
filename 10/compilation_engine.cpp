#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include "compilation_engine.hpp"
using namespace std;

void BaseCompilation::writeSymbol() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::Symbol) {
    que.pop();
    for (int i=0; i<2*indent; i++) *ofs << ' ';
    *ofs << "<symbol>" << ' ';
    *ofs << p.first << ' ';
    *ofs << "</symbol>" << "\n";
  } else error("writeSymbol");
}

void BaseCompilation::writeKeyword() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::Keyword) {
    que.pop();
    for (int i=0; i<2*indent; i++) *ofs << ' ';
    *ofs << "<keyword>" << ' ';
    *ofs << p.first << ' ';
    *ofs << "</keyword>" << "\n";
  } else error("writeSymbol");
}

void BaseCompilation::writeIdentifier() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::Identifier) {
    que.pop();
    for (int i=0; i<2*indent; i++) *ofs << ' ';
    *ofs << "<identifier>" << ' ';
    *ofs << p.first << ' ';
    *ofs << "</identifier>" << "\n";
  } else error("writeIdentifier");
}

void BaseCompilation::writeIntConst() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::IntConst) {
    que.pop();
    for (int i=0; i<2*indent; i++) *ofs << ' ';
    *ofs << "<integerConstant>" << ' ';
    *ofs << p.first << ' ';
    *ofs << "</integerConstant>" << "\n";
  } else error("writeIntConst");
}

void BaseCompilation::writeSringConst() {
  P p = que.front();
  if (p.second == TypeTerminalSymbol::StringConst) {
    que.pop();
    for (int i=0; i<2*indent; i++) *ofs << ' ';
    *ofs << "<stringConstant>" << ' ';
    *ofs << p.first << ' ';
    *ofs << "</stringConstant>" << "\n";
  } else error("writeSringConst");
}

void BaseCompilation::writeType() {
  P p = que.front();
  if (regex_match(p.first, regex(RegexPattern::type))) {
    if (p.second == TypeTerminalSymbol::Keyword) writeKeyword();
    else writeIdentifier();
  } else error("writeType");
}

void BaseCompilation::writeSubroutineCall(bool initalToken) { // doStatement or term
  if (initalToken) writeIdentifier(); // 1. srName 2. srName or className
  if (currentToken() == ".") { // srName or className
    writeSymbol(); // .
    writeIdentifier(); // srName
  } 
  writeSymbol(); // (
  CompilationEngine Ce(que, ofs, indent);
  Ce.compileExpressionList();
  writeSymbol(); // )
}

void TokenClass::compile(){
  writeKeyword(); // "class"
  writeIdentifier(); // className
  writeSymbol(); // {
  int que_size = que.size();
  CompilationEngine Ce(que, ofs, indent);
  while (true) {
   if (TokenClassVarDec::checkInitialToken(currentToken())){
      Ce.compileClassVarDec();
    } else break; 
  }
  while (true) {
   if (TokenSubroutine::checkInitialToken(currentToken())){
      Ce.compileSubroutine();
    } else break; 
  }
  writeSymbol(); // }
}

void TokenClassVarDec::compile() {
  writeKeyword();
  writeType();
  writeIdentifier();
  while(true) {
    if (currentToken() == ",") {
      writeSymbol();
      writeIdentifier();
    } else if (currentToken() == ";") {
      writeSymbol();
      break;
    } else {
      error("in class var dec");
      break;
    }
  }
}

void TokenSubroutine::compile() {
  writeKeyword(); // constructor, function or method
  if (currentToken() == "void") writeKeyword();
  else writeType(); // type
  writeIdentifier(); // subroutiename
  writeSymbol(); // (
  CompilationEngine Ce(que, ofs, indent);
  Ce.compileParameterList();
  writeSymbol(); // )
  Ce.compileSubroutineBody();
}

void TokenSubroutineBody::compile() {
  writeSymbol(); // {
  CompilationEngine Ce(que, ofs, indent);
  while (true) {
    if (!TokenVarDec::checkInitialToken(currentToken())) break;
    Ce.compileVarDec();
  }
  Ce.compileStatements();
  writeSymbol();
}

void TokenParameterList::compile() {
  if (!checkInitialToken(currentToken())) return;
  writeType();
  writeIdentifier();
  while(true) {
    if (currentToken() == ",") {
      writeSymbol();
      writeType();
      writeIdentifier();
    } else break;
  }
}

void TokenVarDec::compile() {
  writeKeyword(); // var
  writeType();
  writeIdentifier();
  while(true) {
    if (currentToken() == ",") {
      writeSymbol();
      writeIdentifier();
    } else if (currentToken() == ";") {
      writeSymbol();
      break;
    } else {
      error("in var dec");
      break;
    }
  }
}

void TokenStatements::compile() {
  CompilationEngine Ce(que, ofs, indent);
  string keyword;
  while(true) {
    keyword = currentToken();
    if (TokenDo::checkInitialToken(keyword)) Ce.compileDo();
    else if (TokenLet::checkInitialToken(keyword)) Ce.compileLet();
    else if (TokenWhile::checkInitialToken(keyword)) Ce.compileWhile();
    else if (TokenIf::checkInitialToken(keyword)) Ce.compileIf();
    else if (TokenReturn::checkInitialToken(keyword)) Ce.compileReturn();
    else break;
  }
}

void TokenDo::compile() {
  writeKeyword();
  writeSubroutineCall();
  writeSymbol();
}

void TokenLet::compile() {
  writeKeyword(); // let
  writeIdentifier(); // varName
  CompilationEngine Ce(que, ofs, indent);
  if (currentToken() != "=") {
    writeSymbol(); // [
    Ce.compileExpression();
    writeSymbol(); // ]
  }
  writeSymbol(); // =
  Ce.compileExpression();
  writeSymbol(); // ;
}

void TokenWhile::compile() {
  writeKeyword(); // while
  writeSymbol(); // (
  CompilationEngine Ce(que, ofs, indent);
  Ce.compileExpression();
  writeSymbol(); // )
  writeSymbol(); // {
  Ce.compileStatements();
  writeSymbol(); // }
}

void TokenIf::compile(){
  writeKeyword();
  writeSymbol();
  CompilationEngine Ce(que, ofs, indent);
  Ce.compileExpression();
  writeSymbol();
  writeSymbol();
  Ce.compileStatements();
  writeSymbol();
  if (currentToken() == "else") {
    writeKeyword();
    writeSymbol();
    Ce.compileStatements();
    writeSymbol();
  }
}

void TokenReturn::compile() {
  writeKeyword();
  if (currentToken() != ";") {
    CompilationEngine Ce(que, ofs, indent);
    Ce.compileExpression();
  }
  writeSymbol();
}

void TokenTerm::compile() {
  P p = que.front();
  CompilationEngine Ce(que, ofs, indent);
  if (p.second == TypeTerminalSymbol::IntConst) writeIntConst();
  else if (p.second == TypeTerminalSymbol::StringConst) writeSringConst();
  else if (p.second == TypeTerminalSymbol::Keyword) writeKeyword();
  else if (p.second == TypeTerminalSymbol::Identifier) {
    // varName, varName[exp], subroutinecall(先頭はidentifier)
    writeIdentifier();
    if (checkEndToken(currentToken())) return; // varName
    else if (currentToken() == "[") { // [ exp ]
      writeSymbol(); // [
      Ce.compileExpression();
      writeSymbol(); // ]
    } else if (currentToken() == "(" || currentToken() == "."){
      writeSubroutineCall(false); // すでにsubroutineNameを上で使用しているため
    }
  }
  else {
    if (p.first == "(") { // (expression)
      writeSymbol();
      Ce.compileExpression();
      writeSymbol();
    } else { // unaryOp term
      writeSymbol();
      Ce.compileTerm(); // termタグを生成することに注意すること
    }
  }
}

void TokenExpression::compile() {
  CompilationEngine Ce(que, ofs, indent);
  Ce.compileTerm();
  while(TokenTerm::checkEndToken(currentToken())) {
    writeSymbol();
    Ce.compileTerm();
  }
}

void TokenExpressionList::compile() {
  if (TokenExpressionList::checkInitialToken(que.front())) {
    CompilationEngine Ce(que, ofs, indent);
    Ce.compileExpression();
    while(currentToken() == ",") {
      writeSymbol();
      Ce.compileExpression();
    }
  }
}
