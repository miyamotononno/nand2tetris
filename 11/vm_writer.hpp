#ifndef VM_WRITER_HPP
#define VM_WRITER_HPP

#include <string>
#include <fstream>
#include <unordered_map>
#include "constants.hpp"
using namespace std;


class VMWriter {
  private:
    ofstream outputfile;
    VMWriter(const VMWriter&);
    VMWriter& operator=(const VMWriter&);
    int labelNum;
    unordered_map<Segment, string> SegmentMap;
  public:
    unordered_map<string, Command> OperationMap;
    VMWriter(string fileName): outputfile(fileName, ofstream::app){
      // unary opは含まないことに注意
      OperationMap["+"] = Command::Add;
      OperationMap["-"] = Command::Sub;
      OperationMap["="] = Command::Eq;
      OperationMap["&gt;"] = Command::Gt;
      OperationMap["&lt;"] = Command::Lt;
      OperationMap["&amp;"] = Command::And;
      OperationMap["|"] = Command::Or;
      OperationMap["*"] = Command::Mul;
      OperationMap["/"] = Command::Div;

      SegmentMap[Segment::Const] = "constant";
      SegmentMap[Segment::Arg] = "argument";
      SegmentMap[Segment::Local] = "local";
      SegmentMap[Segment::Static] = "static";
      SegmentMap[Segment::This] = "this";
      SegmentMap[Segment::That] = "that";
      SegmentMap[Segment::Pointer] = "pointer";
      SegmentMap[Segment::Temp] = "temp";
      labelNum=0;
    }
    void writePush(Segment seg, int index);
    void writePop(Segment seq, int index);
    void writeArithmetic(Command com);
    void writeLabel(string label);
    void writeGoto(string label);
    void writeIf(string label);
    void writeCall(string name, int nArg);
    void writeFunction(string name, int nArg);
    void writeReturn();
    string getNewLabel();
};

#endif // VM_WRITER_HPP