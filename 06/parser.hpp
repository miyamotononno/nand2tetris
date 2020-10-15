#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <string>
using namespace std;

class Parser {
  private:
    string currentCommand;
    string commandWord;
    ifstream *ifs;
    bool init;
    char cType[4];
    string get_command(string str);
  public:
    Parser(ifstream *file);
    bool hasMoreCommand();
    void advance();
    char commandType();
    string symbol();
    string dest();
    string comp();
    string jump();
};

#endif // PARSER_HPP