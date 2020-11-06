#ifndef JACK_TOKENIZER_HPP
#define JACK_TOKENIZER_HPP

#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;
typedef pair<string, int> P;

class JackTokenizer {
  private:
    ifstream *ifs;
    unordered_map<char, string> SYMBOL_MAP;
    bool splitTokens(string &word, queue<P>& terminalSymbolQue);
    bool setTerminalSymbol(string &word, queue<P>& terminalSymbol);
    bool multiCommentout;
  public:
    JackTokenizer(ifstream *inputFile);
    bool advance(queue<P>& terminalSymbolQue);
};

#endif // JACK_TOKENIZER_HPP