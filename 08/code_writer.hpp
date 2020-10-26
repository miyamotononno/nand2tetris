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
    void writeUnaryOperation(string command);
    void writeCompOperation(string command);
    void pop();
    void push();
    label getNewLabel();
    string getAOperationFromAddress(int address, int bufSize=20);
  public:
    CodeWriter(string fileName): outputfile(fileName, ofstream::app){}
    void setFileName(string vmFileName);
    void writeArithmetic(string command);
    void writePushPop(int command, string segment, int index);
    void writeInit();
    void writeLabel(string label);
    void writeGoTo(string label);
    void writeIf(string label);
    // void writeCall(string functionName, int numArgs);
    // void writeReturn();
    // void writeFunction(string functionName, int numLocals);
};

#endif // CODEWRITER_HPP