#ifndef CODEWRITER_HPP
#define CODEWRITER_HPP

#include <string>
#include <fstream>
using namespace std;
struct label{string from, to;};

class CodeWriter {
  private:
    ofstream outputfile;
    string vmFileName, currentFunctionName;
    CodeWriter(const CodeWriter&);
    CodeWriter& operator=(const CodeWriter&);
    int labelNum=0;
    int returnLabelNum=0;

    void writeToFile();
    template <class Head, class... Tail>
    void writeToFile(Head&& head, Tail&&... tail);

    void writeBinaryOperation(string command);
    void writeUnaryOperation(string command);
    void writeCompOperation(string command);
    void pop();
    void push();
    label getNewLabel(bool IsReturnLabel=false);
    string getAOperation(int address, int bufSize=20);
    string getAOperation(string symbol, int bufSize=20);
    string getLOperation(string label, int bufSize=20);
    string getLabelInFunction(string label);
  public:
    CodeWriter(string fileName): outputfile(fileName, ofstream::app){}
    void setFileName(string vmFileName);
    void writeArithmetic(string command);
    void writePushPop(int command, string segment, int index);
    void writeInit();
    void writeLabel(string label);
    void writeGoTo(string label);
    void writeIf(string label);
    void writeCall(string functionName, int numArgs);
    void writeReturn();
    void writeFunction(string functionName, int numLocals);
};

#endif // CODEWRITER_HPP