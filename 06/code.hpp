#ifndef CODE_HPP
#define CODE_HPP

#include <string>
#include <bitset>
#include <unordered_map>
#include <cmath>
#include <vector>
using namespace std;

class Code {
  unordered_map<string, int> destTable;
  unordered_map<string, int> jumpTable;
  unordered_map<string, bitset<7>> compTable;
  public:
  Code();
  bitset<3> dest(string mnemonic);
  bitset<7> comp(string mnemonic);
  bitset<3> jump(string mnemonic);
};

#endif // CODE_HPP