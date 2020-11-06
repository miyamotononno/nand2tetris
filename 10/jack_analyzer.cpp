#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <queue>
#include "jack_tokenizer.hpp"
#include "compilation_engine.hpp"
#include "constants.hpp"
using namespace std;

string getOutputFileName(string path, bool jt_test) {
  string outputFileName="";
  for (char c: path) {
    if (c=='.') break;
    outputFileName+=c;
    if (c=='/') outputFileName+="compare/";
  }
  outputFileName += jt_test?"T.xml":".xml";
  return outputFileName;
}

int main(int argc, char* argv[]) {

  bool jt_test = false;
  string file_name = argv[1];
  string outputFileName = getOutputFileName(file_name, jt_test);
  ifstream ifs(file_name, ios::in);
  if(!ifs){
    cerr << "Error: file not opened." << "\n";
    return 1;
  }
  JackTokenizer JT(&ifs);
  ifstream ofs2(outputFileName, ios::in);
  if(ofs2) filesystem::remove(outputFileName);
  ofstream ofs(outputFileName);
  
  if (jt_test) ofs << "<tokens>" << "\n";
  
  queue<P> terminalSymbolQue;
  JT.advance(terminalSymbolQue);
  int indent = 0;
  CompilationEngine Ce(terminalSymbolQue, &ofs, indent);
  Ce.compileClass();
  if (jt_test) ofs << "</tokens>" << "\n";
  return 0;
}