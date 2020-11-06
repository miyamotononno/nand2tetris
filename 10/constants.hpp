#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

using namespace std;

enum struct TypeTerminalSymbol : int {
  Keyword, Symbol, Identifier, IntConst, StringConst
};

namespace RegexPattern {
  static char* keyword = "class|constructor|function|method|field|static|var|int|char|boolean|void|true|false|null|this|let|do|if|else|while|return";
  static char* stringConstant = "^\".*\"$";
  static char* integerConstant = "^[0-9]+$"; // ここでは厳密に32767以下にはしていない
  static char* identifier = "^[A-Za-z_][A-Za-z0-9_]*$";
  static char* type = "int|char|boolean|(^[A-Za-z_][A-Za-z0-9_]*$)";
}

#endif // CONSTANTS_HPP