#ifndef CODEWRITER_HPP
#define CODEWRITER_HPP

#include <string>
#include <fstream>
#include <stack>
using namespace std;
struct label{string from, to;};

class CodeWriter {
  private:
    ofstream outputfile;
    string vmFileName;
    CodeWriter(const CodeWriter&);
    CodeWriter& operator=(const CodeWriter&);
    int label_num;

    void writeToFile();
    template <class Head, class... Tail>
    void writeToFile(Head&& head, Tail&&... tail);
    void writeBinaryOperation(string command);
    void writeUnaryOperation(string command); // 単項演算
    void writeCompOperation(string command);
    void writePopToDedicatedSegment(string segment, int index);
    void writePushFromDedicatedSegment(string segment, int index);
    void pop();
    void push();
    label getNewLabel();
  public:
    CodeWriter(string fileName): outputfile(fileName, ofstream::app){}
    void setFileName(string vmFileName);
    void writeArithmetic(string command);
    void writePushPop(int command, string segment, int index);
};

#endif // CODEWRITER_HPP