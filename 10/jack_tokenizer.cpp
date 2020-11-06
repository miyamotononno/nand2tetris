#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <iostream>
#include <queue>
#include <sstream>
#include "jack_tokenizer.hpp"
#include "constants.hpp"
using namespace std;

JackTokenizer::JackTokenizer(ifstream *inputFile) {
  ifs = inputFile;
  SYMBOL_MAP['{'] = "{";SYMBOL_MAP['}'] = "}";
  SYMBOL_MAP['['] = "[";SYMBOL_MAP[']'] = "]";
  SYMBOL_MAP['('] = "(";SYMBOL_MAP[')'] = ")";
  SYMBOL_MAP['<'] = "&lt;";SYMBOL_MAP['>'] = "&gt;";
  SYMBOL_MAP['.'] = ".";SYMBOL_MAP[','] = ",";
  SYMBOL_MAP['+'] = "+";SYMBOL_MAP['-'] = "-";
  SYMBOL_MAP['*'] = "*";SYMBOL_MAP['/'] = "/";
  SYMBOL_MAP[';'] = ";";SYMBOL_MAP['&'] = "&amp;";
  SYMBOL_MAP['|'] = "|";SYMBOL_MAP['~'] = "~";
  SYMBOL_MAP['='] = "=";
}

bool JackTokenizer::setTerminalSymbol(string &word, queue<P>& terminalSymbolQue) {
  if (word.size()==1 && SYMBOL_MAP[word[0]].size()>0) {
    terminalSymbolQue.push(P(SYMBOL_MAP[word[0]], TYPE_SYMBOL));
  } else if (regex_match(word, regex(Pattern::KEYWORD_PATTERN))) {
    terminalSymbolQue.push(P(word, TYPE_KEYWORD));
  } else if (regex_match(word, regex(Pattern::STRING_CONSTANT_PATTERN))) {
    terminalSymbolQue.push(P(word, TYPE_STRING_CONST));
  } else if (regex_match(word, regex(Pattern::INTEGER_CONSTANT_PATTERN))) {
    terminalSymbolQue.push(P(word, TYPE_INT_CONST));
  } else if (regex_match(word, regex(Pattern::IDENTIFIER_PATTERN))) {
    terminalSymbolQue.push(P(word, TYPE_IDENTIFIER));
  } else return false;
  return true;
}

bool JackTokenizer::splitTokens(string &word, queue<P>& terminalSymbolQue) { // 再帰的に行う
  bool ok = true;
  P terminalSymbol;
  if (setTerminalSymbol(word, terminalSymbolQue)) return true;
  else {
    smatch m;
    string newWord="";
    for (char c: word) {
      if (SYMBOL_MAP[c].size()>0) {
        setTerminalSymbol(newWord, terminalSymbolQue);
        terminalSymbolQue.push(P(SYMBOL_MAP[c], TYPE_SYMBOL));
        newWord="";
      } else newWord+=c;
    }
    setTerminalSymbol(newWord, terminalSymbolQue);
  }
  return ok;
}

bool JackTokenizer::advance(queue<P>& terminalSymbolQue) {
  bool moreTokens=false;
  string tmp;
  while (getline(*ifs, tmp)) {
    moreTokens = true;
    stringstream ss;
    string stringConstant, str;
    bool InDoublequote=false;
    ss << tmp;
    while (ss >> str) {
      if (str.size()>=2 && str[0]=='/' && str[1]=='/') break;
      if (str == "/**" || str == "/*") {
        multiCommentout = true;
        continue;
      }
      if (str == "*/") {
        multiCommentout = false;
        continue;
      }
      if (multiCommentout) continue;

      if (str=="\"") {
        InDoublequote = !InDoublequote;
        continue;
      }
      if (InDoublequote) {
        stringConstant+=" ";
        string::size_type pos = str.find("\"");
        if (pos==string::npos) stringConstant+=str;
        else {
          string endStringConstant= str.substr(0, pos);
          InDoublequote = false;
          stringConstant+=endStringConstant;
          terminalSymbolQue.push(P(stringConstant, TYPE_STRING_CONST));
          string tokens = str.substr(pos+1);
          splitTokens(tokens, terminalSymbolQue);
        }
      } else {
        if (setTerminalSymbol(str, terminalSymbolQue)) continue;
        string::size_type pos = str.find("\"");
        if (pos!=string::npos) {
          string tokens = str.substr(0, pos);
          splitTokens(tokens, terminalSymbolQue);
          stringConstant = str.substr(pos+1);
          InDoublequote = true;
        } else {
          splitTokens(str, terminalSymbolQue);
        }
      }
    }
    
  }
  return moreTokens;
}
