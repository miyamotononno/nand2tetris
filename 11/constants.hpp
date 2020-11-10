#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

using namespace std;

namespace RegexPattern {
  static char* keyword = "class|constructor|function|method|field|static|var|int|char|boolean|void|true|false|null|this|let|do|if|else|while|return";
  static char* stringConstant = "^\".*\"$";
  static char* integerConstant = "^[0-9]+$"; // ここでは厳密に32767以下にはしていない
  static char* identifier = "^[A-Za-z_][A-Za-z0-9_]*$";
  static char* type = "int|char|boolean|(^[A-Za-z_][A-Za-z0-9_]*$)";
}

enum struct TypeTerminalSymbol {
  Keyword, Symbol, Identifier, IntConst, StringConst
};

enum struct Segment {
  Const, Arg, Local, Static, This, That, Pointer, Temp
};

enum struct Command {
  Add, Sub, Neg, Eq, Gt, Lt, And, Or, Not, Mul, Div
};

enum struct IdentifierAttribute {
  Static, Field, Arg, Var, None
};

struct Identifier {
  string type;
  IdentifierAttribute kind;
  int index;
};

#endif // CONSTANTS_HPP