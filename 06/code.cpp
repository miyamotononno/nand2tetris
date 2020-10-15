#include <string>
#include <bitset>
#include <unordered_map>
#include <cmath>
#include <vector>
#include "code.hpp"
using namespace std;
typedef bitset<7> bit7;
typedef bitset<3> bit3;

Code::Code() {
    destTable["M"] = 1;
    destTable["D"] = 2; destTable["MD"] = 3;
    destTable["A"] = 4; destTable["AM"] = 5;
    destTable["AD"] = 6; destTable["AMD"] = 7;
    jumpTable["JGT"] = 1;
    jumpTable["JEQ"] = 2; jumpTable["JGE"] = 3;
    jumpTable["JLT"] = 4; jumpTable["JNE"] = 5;
    jumpTable["JLE"] = 6; jumpTable["JMP"] = 7;
    compTable["0"] = bit7("0101010");compTable["1"] = bit7("0111111");
    compTable["D"] = bit7("0001100");compTable["-1"] = bit7("0111010");
    compTable["A"] = bit7("0110000");compTable["M"] = bit7("1110000");
    compTable["-D"] = bit7("0001111");compTable["!D"] = bit7("0001101");
    compTable["-A"] = bit7("0110001");compTable["-M"] = bit7("1110001");
    compTable["!A"] = bit7("0110001");compTable["!M"] = bit7("1110001");
    compTable["D+1"] = bit7("0011111");compTable["D-1"] = bit7("0001110");
    compTable["A+1"] = bit7("0110111");compTable["M+1"] = bit7("1110111");
    compTable["A-1"] = bit7("0110010");compTable["M-1"] = bit7("1110010");
    compTable["D+A"] = bit7("0000010");compTable["D+M"] = bit7("1000010");
    compTable["D-A"] = bit7("0010011");compTable["D-M"] = bit7("1010011");
    compTable["A-D"] = bit7("0000111");compTable["M-D"] = bit7("1000111");
    compTable["D&A"] = bit7("0000000");compTable["D&M"] = bit7("1000000");
    compTable["D|A"] = bit7("0010101");compTable["D|M"] = bit7("1010101");
}

bit3 Code::dest(string mnemonic) {
  if (!destTable[mnemonic]) return bit3(0);
  bit3 bs(destTable[mnemonic]);
  return bs;
}

bit7 Code::comp(string mnemonic) {
  return compTable[mnemonic];
}

bit3 Code::jump(string mnemonic) {
  if (!jumpTable[mnemonic]) return bit3(0);
  bit3 bs(jumpTable[mnemonic]);
  return bs;
}