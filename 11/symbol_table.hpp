#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>
#include "constants.hpp"
using namespace std;

class SymbolTable {
  private:
    int index;
    bool currentScope; // class: True, subroutine: False
    unordered_map<string, Identifier> classTable;
    unordered_map<string, Identifier> subroutineTable;
    unordered_map<string, Identifier> getScopeTable();
    static bool isClass(IdentifierAttribute kind) {
      return (kind == IdentifierAttribute::Static || kind == IdentifierAttribute::Field);
    };
  public:
    SymbolTable():currentScope(true){}
    string currentClassName;
    void startSubroutine();
    void define(string name, string type, IdentifierAttribute kind);
    int varCount(IdentifierAttribute kind);
    IdentifierAttribute kindOf(string name);
    string typeOf(string name);
    int indexOf(string name);
};

inline unordered_map<string, Identifier> SymbolTable::getScopeTable() {
  return currentScope?classTable:subroutineTable;
}


#endif // SYMBOL_TABLE_HPP