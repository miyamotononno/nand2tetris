#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <queue>
#include "jack_tokenizer.hpp"
#include "constants.hpp"
using namespace std;

string getOutputFileName(string path) {
  string outputFileName="";
  for (char c: path) {
    if (c=='.') break;
    outputFileName+=c;
    if (c=='/') outputFileName+="compare/";
  }
  outputFileName += "T.xml";
  return outputFileName;
}

int main(int argc, char* argv[]) {

  bool jt_test = true;
  string file_name = argv[1];
  string outputFileName = getOutputFileName(file_name);
  ifstream ifs(file_name, ios::in);
  if(!ifs){
    cerr << "Error: file not opened." << "\n";
    return 1;
  }
  JackTokenizer JT(&ifs);
  queue<P> terminalSymbolQue;
  ifstream ofs2(outputFileName, ios::in);
  if(ofs2) filesystem::remove(outputFileName);
  ofstream ofs(outputFileName);
  
  if (jt_test) ofs << "<tokens>" << "\n";
  
  while(JT.advance(terminalSymbolQue)) {
    while(!terminalSymbolQue.empty()) {
      P pairTerminalSymbol = terminalSymbolQue.front(); terminalSymbolQue.pop();
      string terminalSymbol = pairTerminalSymbol.first;
      int tokenType = pairTerminalSymbol.second;
      string startTag, endTag;
      switch(tokenType) {
        case TYPE_KEYWORD:
          startTag="<keyword>";
          endTag = "</keyword>";
          break;
        case TYPE_SYMBOL:
          startTag="<symbol>";
          endTag = "</symbol>";
          if (terminalSymbol == "<") terminalSymbol = "&lt";
          if (terminalSymbol == ">") terminalSymbol = "&gt";
          if (terminalSymbol == "&") terminalSymbol = "&amp";
          break;
        case TYPE_IDENTIFIER:
          startTag="<identifier>";
          endTag = "</identifier>";
          break;
        case TYPE_INT_CONST:
          startTag="<integerConstant>";
          endTag = "</integerConstant>";
          break;
        case TYPE_STRING_CONST:
          startTag="<stringConstant>";
          endTag = "</stringConstant>";
          break;
      }
      ofs << startTag << ' ';
      ofs << terminalSymbol << ' ';
      ofs << endTag << "\n";
    }
  }
  if (jt_test) {
    ofs << "</tokens>" << "\n";
    return 0;
  }
  return 0;
}