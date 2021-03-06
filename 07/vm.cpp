#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "parser.hpp"
#include "code_writer.hpp"
#include "constants.hpp"
using namespace std;
namespace fs = std::filesystem;

string get_output_file_name(string file_name) {
  string vmFile;
  for (char c: file_name) {
    if (c=='.') break;
    vmFile+=c;
  }
  return vmFile;
}

int main(int argc, char* argv[]) {
  string file_name = argv[1];
  ifstream ifs(file_name, ios::in);
  if(!ifs){
    cerr << "Error: file not opened." << "\n";
    return 1;
  }
  string vmFileName = get_output_file_name(file_name);
  string output_file_name = vmFileName + ".asm";
  ifstream ofs(output_file_name, ios::in);
  if(ofs) fs::remove(output_file_name);
  CodeWriter Cw(output_file_name);
  Cw.setFileName(vmFileName);
  Parser Parser(&ifs);
  while(Parser.advance()) {
    int cType = Parser.commandType();
    if (cType == C_RETURN) continue; 
    if (cType == C_ARITHMETIC) {
      string command = Parser.arg1();
      Cw.writeArithmetic(command);
    } else if (cType==C_POP || cType==C_PUSH) {
      string segment = Parser.arg1();
      int index = Parser.arg2();
      Cw.writePushPop(cType, segment, index);
    }
  }
  return 0;
}