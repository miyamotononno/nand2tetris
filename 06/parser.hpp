#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <string>
using namespace std;

class Parser {
  bool moreCommand;
  string command;
  ifstream *ifs;
  bool init;
  int COMMAND_A_TYPE;
  int COMMAND_C_TYPE;
  int COMMAND_L_TYPE;
  char A_COMMAND_PATTERN[1025], L_COMMAND_PATTERN[1025];
  public:
    Parser(ifstream *file, int A_TYPE=1, int C_TYPE=2, int L_TYPE=3);
    bool advance();
    int commandType();
    string symbol();
    string dest();
    string comp();
    string jump();
};

#endif // PARSER_HPP