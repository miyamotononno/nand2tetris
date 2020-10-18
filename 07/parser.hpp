#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <string>
using namespace std;

class Parser {
  string command;
  string first_arg;
  string second_arg;
  ifstream *ifs;
  int cType;
  void split_command(string &command);
  int C_ARITHMETIC;
  int C_PUSH;
  int C_POP;
  int C_LABEL;
  int C_GOTO;
  int C_IF;
  int C_FUNCTION;
  int C_RETURN;
  int C_CALL;
  string args[4];
  public:
    Parser(ifstream *file, int C_ARITHMETIC=1, int C_PUSH=2,
           int C_POP=3, int C_LABEL=4, int C_GOTO=5, int C_IF=6,
           int C_FUNCTION=7, int C_RETURN=9, int C_CALL=10);
    bool advance();
    int commandType();
    string arg1();
    int arg2();
  
};

#endif // PARSER_HPP