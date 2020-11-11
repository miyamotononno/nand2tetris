#include "symbol_table.hpp"
using namespace std;

void SymbolTable::startSubroutine() {
  argTable.clear();
  varTable.clear();
}

void SymbolTable::define(string name, string type, IdentifierAttribute kind) {
  if (kind == IdentifierAttribute::Arg) argTable[name] = { type, kind, varCount(kind) };
  if (kind == IdentifierAttribute::Field) fieldTable[name] = { type, kind, varCount(kind) };
  if (kind == IdentifierAttribute::Static) staticTable[name] = { type, kind, varCount(kind) };
  if (kind == IdentifierAttribute::Var) varTable[name] = { type, kind, varCount(kind) };
}

int SymbolTable::varCount(IdentifierAttribute kind) {
  if (kind == IdentifierAttribute::Arg) return argTable.size();
  if (kind == IdentifierAttribute::Field) return fieldTable.size();
  if (kind == IdentifierAttribute::Static) return staticTable.size();
  if (kind == IdentifierAttribute::Var) return varTable.size();
  return 0;
}

IdentifierAttribute SymbolTable::kindOf(string name) {
  Identifier identifier = findByName(name);
  return identifier.kind;
}

string SymbolTable::typeOf(string name) {
  Identifier identifier = findByName(name);
  return identifier.type;
}

int SymbolTable::indexOf(string name) {
  Identifier identifier = findByName(name);
  return identifier.index;
}

Identifier SymbolTable::findByName(string name) {
  if (argTable.find(name) != argTable.end()) return argTable[name];
  if (fieldTable.find(name) != fieldTable.end()) return fieldTable[name];
  if (staticTable.find(name) != staticTable.end()) return staticTable[name];
  if (varTable.find(name) != varTable.end()) return varTable[name];
  return Identifier{"", IdentifierAttribute::None, -1};
}