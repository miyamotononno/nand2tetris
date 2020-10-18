#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include "parser.hpp"
using namespace std;

Parser::Parser(ifstream *file, int _C_ARITHMETIC, int _C_PUSH,
               int _C_POP, int _C_LABEL, int _C_GOTO, int _C_IF,
               int _C_FUNCTION, int _C_RETURN, int _C_CALL) {
  ifs = file;
  C_ARITHMETIC=_C_ARITHMETIC;
  C_PUSH=_C_PUSH;
  C_POP=_C_POP;
  C_LABEL=_C_LABEL;
  C_GOTO=_C_GOTO;
  C_IF=_C_IF;
  C_FUNCTION=_C_FUNCTION;
  C_RETURN=_C_RETURN;
  C_CALL=_C_CALL;
  args[0]="";
  args[1]="";
  args[2]="";
}

void Parser::split_command(string &command) {
  if (command.size()<=1) return;
  
  string com;
  int idx=0;
  for (char c: command) {
    if (c==' ') {
      args[idx] = com;
      com="";
      idx++;
      if (idx==3) return;
      continue;
    }
    com+=c;
  }
  args[idx] = com;
}

bool Parser::advance() {
  string buf;
  bool moreCommand=false;
  if (getline(*ifs, buf)) {
    moreCommand = true;
    const string commentout("//");
    string::size_type pos = buf.find(commentout);
    if (pos!=string::npos) buf = buf.substr(0, pos);
    split_command(buf);
  }
  return moreCommand;
}

int Parser::commandType(){
  string arg1=args[0];
  cType = 0;
  
  if (arg1=="push") cType=C_PUSH;
  else if (arg1=="pop") cType=C_POP;
  else if (arg1=="label") cType=C_LABEL;
  else if (arg1=="goto") cType=C_GOTO;
  else if (arg1=="if-goto") cType=C_IF;
  else if (arg1=="function") cType=C_FUNCTION;
  else if (arg1=="call") cType=C_CALL;
  else if (arg1=="return") cType=C_RETURN;
  else if (arg1=="add" ||
           arg1=="sub" ||
           arg1=="neg" ||
           arg1=="eq" ||
           arg1=="gt" ||
           arg1=="lt" ||
           arg1=="and" ||
           arg1=="or" ||
           arg1=="not") cType=C_ARITHMETIC;
  return cType;
}

string Parser::arg1(){
  if (cType==C_RETURN) cerr << "invalid command type(RETURN)" << "\n";
  if (cType==C_ARITHMETIC) return args[0];
  return args[1];
}

int Parser::arg2(){
  if (cType==C_PUSH ||
      cType==C_POP ||
      cType==C_FUNCTION ||
      cType==C_CALL) {
    return stoi(args[2]);
  }
  
  cerr << "invalid command type" << "\n";
  return 0;
}

