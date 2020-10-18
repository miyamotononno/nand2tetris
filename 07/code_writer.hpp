#ifndef CODEWRITER_HPP
#define CODEWRITER_HPP

#include <string>
#include <bitset>
#include <fstream>
using namespace std;

class CodeWriter {
  ifstream *ifs;
  public:
    CodeWriter(ifstream *file);
    void setFileName(string fileName);
    void writeArithmetic(string command);
    void writePushPop(string command, string segment, int index);
    void close();
};

#endif // CODEWRITER_HPP