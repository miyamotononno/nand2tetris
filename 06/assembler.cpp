#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <stdio.h> 
#include <string.h> 
#include <vector>
#include "parser.hpp"
#include "code.hpp"
using namespace std;

class SymbolTable {
  unordered_map<string, int> table;

  public:
    SymbolTable() {
      table["SP"] = 0;table["LCL"] = 1;table["ARG"] = 2;
      table["THIS"] = 3;table["THAT"] = 4;
      table["R0"] = 0;table["R1"] = 1;table["R2"] = 2;table["R3"] = 3;
      table["R4"] = 4;table["R5"] = 5;table["R6"] = 6;table["R7"] = 7;
      table["R8"] = 8;table["R9"] = 9;table["R10"] = 10;table["R11"] = 11;
      table["R12"] = 12;table["R13"] = 13;table["R14"] = 14;table["R15"] = 15;
      table["SCREEN"] = 16384;table["KBD"] = 24576;
    }
    void addEntry(string symbol, int address) {
      table[symbol] = address;
    }
    bool contains(string symbol) {
      return symbol=="SP" || symbol=="R0" || table[symbol] > 0;
    }
    int getAddress(string symbol) {
      return table[symbol];
    }
};

int string_to_int(string str) {
  int digit=0;
  int ret = 0;
  for (int i=str.size()-1; i>=0; i--) {
    int x = str[i]-'0';
    if (x>=10) return -1;
    ret+=x*pow(10, digit);
    digit++;
  }
  return ret;
}

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

  while(firstParse.hasMoreCommand()) {
    firstParse.advance();
    char ctype = firstParse.commandType();
    if (ctype=='L') {
      string symbol = firstParse.symbol();
      if (!ST.contains(symbol)) ST.addEntry(symbol, idx);
    }
    if (ctype=='A' || ctype=='C') idx++;
  }




  ifstream ifs2(file_name, ios::in);
  Parser secondParse(&ifs2);
  Code C = Code();
  string output_file_name = get_output_file_name(file_name);
  ofstream outputfile(output_file_name);
  int registerIdx = 16;
  while(secondParse.hasMoreCommand()) {
    secondParse.advance();
    char ctype = secondParse.commandType();
    switch(ctype) {
      case 'C': {
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
      case 'A': {
        string ps = secondParse.symbol();
        int d = string_to_int(ps);
        if (d<0) {
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