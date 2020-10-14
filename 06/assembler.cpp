#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <stdio.h> 
#include <string.h> 
#include <vector>
using namespace std;
typedef bitset<7> bit7;
typedef bitset<3> bit3;

class Parser {
  string currentCommand;
  string commandWord;
  ifstream *ifs;
  bool init;
  char cType[4];
  string get_command(string str) { 
    string word = "";
    for (auto x : str){
        if (x == ' ') {
          if (word=="//") break;
          return word;
        }
        else word = word + x;
    }
    return word;
  }
  public:
    
  Parser(ifstream *file) { // pointerで渡す
    ifs = file;
    cType[0] = 'A';
    cType[1] = 'C';
    cType[2] = 'L';
    cType[3] = 'N';
    init=true;
  }

  bool hasMoreCommand() {
    return init || currentCommand.size()>0;
  }

  void advance() {
    string buf;
    getline(*ifs, buf);
    currentCommand = buf;
    commandWord = get_command(currentCommand);
    commandWord = commandWord.substr(0, commandWord.size()-1); // 最後空文字あるので
    init = false;
  }
  char commandType() {
    int ctype;
    if (commandWord.size()<=1 || commandWord[0] == '/') ctype=3;
    else if (commandWord[0] == '(') ctype=2;
    else if (commandWord[0]=='@') ctype=0;
    else ctype=1;
    return cType[ctype];
  }
  string symbol() {
    char ct = commandType();
    if (ct=='A') return commandWord.substr(1);
    else return commandWord.substr(1, commandWord.size()-2); // L
  }

  string dest() {
    string ret = "";
    for (char c: commandWord) {
      if (c=='=') {
        break;
      } else if (c==';') return ""; // destは存在しない
      ret+=c;
    }
    return ret;
  }

  string comp() {
    string ret = "";
    for (char c: commandWord) {
      if (c==';') return ret;  
      else if (c=='=') {
        ret="";
        continue;
      }
      ret+=c;
    }
    return ret;
  }

  string jump() {
    string ret = "";
    bool ok = false;
    for (char c: commandWord) {
      if (c==';') {
        ok=true;
        continue;
      }
      else if (c=='=') return ""; // jumpは存在しない
      if (ok) ret+=c;
    }
    return ret;
  }
};

class Code {
  unordered_map<string, int> destTable;
  unordered_map<string, int> jumpTable;
  unordered_map<string, bit7> compTable;
  public:
  Code() {
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
  bit3 dest(string mnemonic) {
    if (!destTable[mnemonic]) return bit3(0);
    bit3 bs(destTable[mnemonic]);
    return bs;
  }

  bit7 comp(string mnemonic) {
    return compTable[mnemonic];
  }

  bit3 jump(string mnemonic) {
    if (!jumpTable[mnemonic]) return bit3(0);
    bit3 bs(jumpTable[mnemonic]);
    return bs;
  }
};

class SymbolTable {
  unordered_map<string, int> table;
  public:
    void addEntry(string symbol, int address) {
      table[symbol] = address;
    }

    bool contains(string symbol) {
      return table[symbol] > 0;
    }

    int getAddress(string symbol) {
      return table[symbol];
    }
};

int string_to_int(string str) {
  int digit=0;
  int ret = 0;
  for (int i=str.size()-1; i>=0; i--) {
    ret+=(str[i]-'0')*pow(10, digit);
    digit++;
  }
  return ret;
}

int main(int argc, char* argv[]) {
  string file_name = argv[1];
  ifstream ifs(file_name, ios::in);
  if(!ifs){
    cerr << "Error: file not opened." << endl;
    return 1;
  }
  Parser parse(&ifs);
  Code C = Code();
  // SymbolTable ST = SymbolTable();
  string output_file_name;
  for (char c: file_name) { 
    output_file_name+=c;
    if (c=='.') break;
  }
  output_file_name+="hack";
  ofstream outputfile(output_file_name);
  while(parse.hasMoreCommand()) {
    parse.advance();
    char ctype = parse.commandType();
    switch(ctype) {
      case 'C': {
        string d = parse.dest();
        string c = parse.comp();
        string j = parse.jump();
        bit3 dest = C.dest(d);
        bit7 comp = C.comp(c);
        bit3 jump = C.jump(j);
        outputfile << 111 << comp << dest << jump << "\n";
        break;
      }
      case 'A':
      case 'L': {
        string ps = parse.symbol();
        bitset<15> b(string_to_int(ps));
        //　取りあえすシンボルなし。10進数のみ
        outputfile << 0 << b << "\n";
      }
    }
  }

  return 0;
}