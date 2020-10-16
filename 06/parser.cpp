#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include "parser.hpp"
using namespace std;
char SYMBOL_PATTERN[] = "([0-9a-zA-Z_\.\$:]+)";

Parser::Parser(ifstream *file, int A_TYPE, int C_TYPE, int L_TYPE) { // pointerで渡す
    ifs = file;
    init=true;
    COMMAND_A_TYPE=A_TYPE;
    COMMAND_C_TYPE=C_TYPE;
    COMMAND_L_TYPE=L_TYPE;
    sprintf(A_COMMAND_PATTERN, "@%s", SYMBOL_PATTERN);
    sprintf(L_COMMAND_PATTERN, "[(]%s[)]", SYMBOL_PATTERN);
}

bool Parser::advance() {
  string buf;
  if (getline(*ifs, buf)) {
    moreCommand = true;
    const string commentout("//");
    string::size_type pos = buf.find(commentout);
    if (pos!=string::npos) buf = buf.substr(0, pos);
    buf=regex_replace(buf, regex("[ \t\f\r\n]"), "");
    command = buf;
    init = false;
  } else moreCommand = false;
  return moreCommand;
}

// 正規表現に直すこと

int Parser::commandType(){
  if (command.size()==0) return 0;
  if (regex_match(command, regex(A_COMMAND_PATTERN))) return COMMAND_A_TYPE;
  if (regex_match(command, regex(L_COMMAND_PATTERN))) return COMMAND_L_TYPE;
  return COMMAND_C_TYPE;
}

string Parser::symbol() {
  int ct = commandType();
  if (ct==COMMAND_A_TYPE) command=command.substr(1);
  else if (ct==COMMAND_L_TYPE) command=command.substr(1, command.find(')')-1);
  else cerr << "AコマンドでもLコマンドでもない" << endl;
  return command;
}

string Parser::dest() {
  string ret = "";
  for (char c: command) {
    if (c=='=') {
      break;
    } else if (c==';') return ""; // destは存在しない
    ret+=c;
  }
  return ret;
}

string Parser::comp() {
  string ret = "";
  for (char c: command) {
    if (c==';') return ret;  
    else if (c=='=') {
      ret="";
      continue;
    }
    ret+=c;
  }
  return ret;
}

string Parser::jump() {
  string ret = "";
  bool ok = false;
  for (char c: command) {
    if (c==';') {
      ok=true;
      continue;
    }
    else if (c=='=') return ""; // jumpは存在しない
    if (ok) ret+=c;
  }
  return ret;
}