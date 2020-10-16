#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>
#include <fstream>
#include <cmath>
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
    void addEntry(string symbol, int address);
    bool contains(string symbol);
    int getAddress(string symbol);
};

inline void SymbolTable::addEntry(string symbol, int address) {
  table[symbol] = address;
}

inline bool SymbolTable::contains(string symbol) {
  return symbol=="SP" || symbol=="R0" || table[symbol] > 0;
}

inline int SymbolTable::getAddress(string symbol) {
  return table[symbol];
}

