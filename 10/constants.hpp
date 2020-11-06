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
  public:
    static inline const char* KEYWORD_PATTERN = "class|constructor|function|method|field|static|var|int|char|boolean|void|true|false|null|this|let|do|if|else|while|return";
    static inline const char* STRING_CONSTANT_PATTERN = "^\".*\"$";
    static inline const char* INTEGER_CONSTANT_PATTERN = "^[0-9]+$"; // ここでは厳密に32767以下にはしていない
    static inline const char* IDENTIFIER_PATTERN = "^[A-Za-z_][A-Za-z0-9_]*$";
    static inline const char* CLASSNAME_PATTERN=IDENTIFIER_PATTERN;
    static inline const char* SUBROUTINE_NAME_PATTERN=IDENTIFIER_PATTERN;
    static inline const char* VAR_NAME_PATTERN=IDENTIFIER_PATTERN;
    static inline const char* TYPE_PATTERN = "int|char|boolean|(^[A-Za-z_][A-Za-z0-9_]*$)";
};

#endif // CONSTANTS_HPP