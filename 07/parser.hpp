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
  string args[4];
  public:
    Parser(ifstream *file);
    bool advance();
    int commandType();
    string arg1();
    int arg2();
  
};

#endif // PARSER_HPP