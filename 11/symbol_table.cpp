#include <string>
#include <unordered_map>
#include "symbol_table.hpp"
using namespace std;

void SymbolTable::startSubroutine() {
  subroutineTable.clear();
  index = 0;
  currentScope = false;
}

void SymbolTable::define(string name, string type, IdentifierAttribute kind) {
  if (isClass(kind)) {
    currentScope = true;
    classTable[name] = { type, kind, index };
  } else {
    subroutineTable[name] = { type, kind, index };
  }
  index++;
}

int SymbolTable::varCount(IdentifierAttribute kind) {
  if (isClass(kind) == currentScope) return index;
  else return 0;
}

IdentifierAttribute SymbolTable::kindOf(string name) {
  unordered_map<string, Identifier> scopeTable = getScopeTable();
  if (scopeTable.find(name) == scopeTable.end()) {
    return IdentifierAttribute::None;
  } return scopeTable[name].kind;
}

string SymbolTable::typeOf(string name) {
  unordered_map<string, Identifier> scopeTable = getScopeTable();
  return scopeTable[name].type;
}

int SymbolTable::indexOf(string name) {
  unordered_map<string, Identifier> scopeTable = getScopeTable();
  return scopeTable[name].index;
}