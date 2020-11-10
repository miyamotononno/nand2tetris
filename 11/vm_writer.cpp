#include <string>
#include <fstream>
#include "vm_writer.hpp"
using namespace std;

void VMWriter::writePush(Segment seg, int index) {
  outputfile << "push" << ' ';
  outputfile << SegmentMap[seg] << ' ';
  outputfile << index << "\n";
}

void VMWriter::writePop(Segment seg, int index) {
  outputfile << "pop" << ' ';
  outputfile << SegmentMap[seg] << ' ';
  outputfile << index << "\n";
}

void VMWriter::writeArithmetic(Command com) {
  switch(com) {
    case Command::Add:
      outputfile << "add" << "\n";
      break;
    case Command::Sub:
      outputfile << "sub" << "\n";
      break;
    case Command::Neg:
      outputfile << "neg" << "\n";
      break;
    case Command::Eq:
      outputfile << "eq" << "\n";
      break;
    case Command::Gt:
      outputfile << "gt" << "\n";
      break;
    case Command::Lt:
      outputfile << "lt" << "\n";
      break;
    case Command::And:
      outputfile << "and" << "\n";
      break;
    case Command::Or:
      outputfile << "or" << "\n";
      break;
    case Command::Not:
      outputfile << "not" << "\n";
      break;
    case Command::Mul:
      writeCall("Math.multiply", 2);
      break;
    case Command::Div:
      writeCall("Math.divide", 2);
      break;
    default:
      break;
  }
}

void VMWriter::writeLabel(string label) {
  outputfile << "label" << ' ' << label << "\n";
}

void VMWriter::writeGoto(string label) {
  outputfile << "goto" << ' ' << label << "\n";
}

void VMWriter::writeIf(string label) {
  outputfile << "if-goto" << ' ' << label << "\n";
}

void VMWriter::writeCall(string name, int nArg) {
  outputfile << "call" << ' ';
  outputfile << name << ' ';
  outputfile << nArg << "\n";
}

void VMWriter::writeFunction(string name, int nLocals) {
  outputfile << "function" << ' ';
  outputfile << name << ' ';
  outputfile << nLocals << "\n";
}

void VMWriter::writeReturn() {
  outputfile << "return" << "\n";
}

string VMWriter::getNewLabel() {
  labelNum++;
  char oper[10];
  sprintf(oper, "L%d", labelNum);
  return oper;
}