#ifndef JACK_TOKENIZER_HPP
#define JACK_TOKENIZER_HPP

#include <string>
#include <fstream>
using namespace std;

class JackTokenizer {
  private:
  public:
    JackTokenizer(ifstream *inputFile);
    bool hasMoreTokens();
    void advance();
    int tokenType();
    string KeyWord();
    char symbol();
    string identifier();
    int intval();
    string stringVal();
};

#endif // JACK_TOKENIZER_HPP