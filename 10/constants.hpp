#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
using namespace std;

const int TYPE_KEYWORD = 1;
const int TYPE_SYMBOL = 2;
const int TYPE_IDENTIFIER = 3;
const int TYPE_INT_CONST = 4;
const int TYPE_STRING_CONST = 5;

class Pattern {
  private:
    // void setPattern();
  public:
    static inline const char* KEYWORD_PATTERN = "class|constructor|function|method|field|static|var|int|char|boolean|void|true|false|null|this|let|do|if|else|while|return";
    static inline const char* STRING_CONSTANT_PATTERN = "^\".*\"$";
    static inline const char* INTEGER_CONSTANT_PATTERN = "^[0-9]+$"; // ここでは厳密に32767以下にはしていない
    static inline const char* IDENTIFIER_PATTERN = "^[A-Za-z_][A-Za-z0-9_]*$";
    // const char* CLASSNAME_PATTERN=IDENTIFIER_PATTERN;
    // const char* SUBROUTINE_NAME_PATTERN=IDENTIFIER_PATTERN;
    // const char* VAR_NAME_PATTERN=IDENTIFIER_PATTERN;
    // char* TYPE_PATTERN;
    // char* PARAMETER_LIST_PATTERN;
    // char* CLASS_VAR_DEC_PATTERN;

    // char* OP_PATTERN = "[\+|-|\*|/|\&|\||<|>|=]";
    // char* UNARY_OP_PATTERN = "[-|~]";
    // char* KEYWORD_OPERATION = "true|false|null|this";

    // expression, term, subroutine call, expression listは再帰的に実装する
   
};

// inline void Pattern::setPattern(){
//   sprintf(TYPE_PATTERN, "int|char|boolean|%s", CLASSNAME_PATTERN);
//   sprintf(PARAMETER_LIST_PATTERN, "(%s\s+%s)(,\s+%s\s+%s)*", TYPE_PATTERN, VAR_NAME_PATTERN, TYPE_PATTERN, VAR_NAME_PATTERN);
//   sprintf(CLASS_VAR_DEC_PATTERN, "(static|field)\s+%s\s+%s,\s+");
// }

#endif // CONSTANTS_HPP