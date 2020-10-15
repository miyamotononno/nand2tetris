#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include "parser.hpp"
using namespace std;

string Parser::get_command(string str) {
  string word = "";
  for (int i=0; i<str.size(); i++) {
      int d = int(str[i]);
      if (d==47) { // /のこと
        if (i<str.size()-1 && str[i+1]==47) break;
      }
      if (d==13) return word;
      if (d==32) continue;
      word = word + str[i];
  }
  return word;
}

Parser::Parser(ifstream *file) { // pointerで渡す
    ifs = file;
    cType[0] = 'A';
    cType[1] = 'C';
    cType[2] = 'L';
    cType[3] = 'N';
    init=true;
}

bool Parser::hasMoreCommand() {
  return init || currentCommand.size()>0;
}

void Parser::advance() {
  string buf;
  getline(*ifs, buf);
  currentCommand = buf;
  commandWord = get_command(currentCommand);
  init = false;
}

char Parser::commandType(){
  int ctype;
  if (commandWord.size()==0) ctype=3;
  else if (commandWord[0] == '(') ctype=2;
  else if (commandWord[0]=='@') ctype=0;
  else ctype=1;
  return cType[ctype];
}

string Parser::symbol() {
  char ct = commandType();
  if (ct=='A') return commandWord.substr(1);
  else return commandWord.substr(1, commandWord.size()-2); // L
}

string Parser::dest() {
  string ret = "";
  for (char c: commandWord) {
    if (c=='=') {
      break;
    } else if (c==';') return ""; // destは存在しない
    ret+=c;
  }
  return ret;
}

string Parser::comp() {
  string ret = "";
  for (char c: commandWord) {
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
  for (char c: commandWord) {
    if (c==';') {
      ok=true;
      continue;
    }
    else if (c=='=') return ""; // jumpは存在しない
    if (ok) ret+=c;
  }
  return ret;
}