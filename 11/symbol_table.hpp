#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <unordered_map>
#include "common.hpp"
using namespace std;

class SymbolTable {
  private:
    unordered_map<string, Identifier> staticTable;
    unordered_map<string, Identifier> fieldTable;
    unordered_map<string, Identifier> argTable;
    unordered_map<string, Identifier> varTable;
    Identifier findByName(string name);
  public:
    string currentClassName;
    void startSubroutine();
    void define(string name, string type, IdentifierAttribute kind);
    int varCount(IdentifierAttribute kind);
    IdentifierAttribute kindOf(string name);
    string typeOf(string name);
    int indexOf(string name);
};

#endif // SYMBOL_TABLE_HPP