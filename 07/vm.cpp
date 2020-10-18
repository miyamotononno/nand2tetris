#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <regex>
#include "parser.hpp"
using namespace std;

const int C_ARITHMETIC=1;
const int C_PUSH=2;
const int C_POP=3;
const int C_LABEL=4;
const int C_GOTO=5;
const int C_IF=6;
const int C_FUNCTION=7;
const int C_RETURN=9;
const int C_CALL=10;

string get_output_file_name(string file_name) {
  string output_file_name;
  for (char c: file_name) { 
    output_file_name+=c;
    if (c=='.') break;
  }
  output_file_name+="asm";
  return output_file_name;
}

int main(int argc, char* argv[]) {
  string file_name = argv[1];
  ifstream ifs(file_name, ios::in);
  if(!ifs){
    cerr << "Error: file not opened." << "\n";
    return 1;
  }
  Parser Parser(&ifs);
  while(Parser.advance()) {
    int cType = Parser.commandType();
    if (cType != C_RETURN) {
      cout << "arg1:" << ' ' << Parser.arg1() << "\n";
      if (cType==C_PUSH || cType==C_POP ||
          cType==C_FUNCTION || cType==C_CALL) {
            cout<< "arg2:" << ' ' << Parser.arg2() << "\n";
          }
    }
  }
  return 0;
}