#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <regex>
#include "parser.hpp"
#include "code.hpp"
#include "symbol_table.hpp"
using namespace std;

const int COMMAND_A_TYPE = 1;
const int COMMAND_C_TYPE = 2;
const int COMMAND_L_TYPE = 3;
char INTEGER_PATTERN[] = "[0-9]+";

string get_output_file_name(string file_name) {
  string output_file_name;
  for (char c: file_name) { 
    output_file_name+=c;
    if (c=='.') break;
  }
  output_file_name+="hack";
  return output_file_name;
}

int main(int argc, char* argv[]) {
  string file_name = argv[1];
  ifstream ifs(file_name, ios::in);
  if(!ifs){
    cerr << "Error: file not opened." << endl;
    return 1;
  }


  Parser firstParse(&ifs);
  SymbolTable ST = SymbolTable();
  int idx=0;

  while(firstParse.advance()) {
    int ctype = firstParse.commandType();
    if (ctype==COMMAND_L_TYPE) {
      string symbol = firstParse.symbol();
      if (!ST.contains(symbol)) ST.addEntry(symbol, idx);
    }
    if (ctype==COMMAND_A_TYPE || ctype==COMMAND_C_TYPE) idx++;
  }


  ifstream ifs2(file_name, ios::in);
  Parser secondParse(&ifs2);
  Code C = Code();
  string output_file_name = get_output_file_name(file_name);
  ofstream outputfile(output_file_name);
  int registerIdx = 16;
  while(secondParse.advance()) {
    int ctype = secondParse.commandType();
    switch(ctype) {
      case COMMAND_C_TYPE: {
        string d = secondParse.dest();
        string c = secondParse.comp();
        string j = secondParse.jump();
        bitset<3> dest = C.dest(d);
        bitset<7> comp = C.comp(c);
        bitset<3> jump = C.jump(j);
        // cout << 111 << comp << dest << jump << "\n";
        outputfile << 111 << comp << dest << jump << "\n";
        break;
      }
      case COMMAND_A_TYPE: {
        string ps = secondParse.symbol();
        int d=0;
        if (regex_match(ps, regex(INTEGER_PATTERN))) d = stoi(ps);
        else {
          if (ST.contains(ps)) d = ST.getAddress(ps);
          else {
            ST.addEntry(ps, registerIdx);
            d = registerIdx;
            registerIdx++;
          }
        }
        bitset<15> b(d);
        // cout << 0 << b << "\n";
        outputfile << 0 << b << "\n";
        break;
      }
    }
  }

  return 0;
}