#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <string>
using namespace std;

class Parser {
  private:
    ifstream ifs;
    Parser(const Parser&);
    Parser& operator=(const Parser&);
    string command;
    string first_arg;
    string second_arg;
    int cType;
    void split_command(string &command);
    string args[4];
  public:
    Parser(string fileName): ifs(fileName, ios::in){}
    bool advance();
    int commandType();
    string arg1();
    int arg2();
};

#endif // PARSER_HPP