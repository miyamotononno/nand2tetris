#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <stdarg.h>
#include "code_writer.hpp"
#include "constants.hpp"
using namespace std;

const int POINTER_SEGMENT = 3;
const int TEMP_SEGEMENT = 5;

void CodeWriter::writeToFile(){};
template <class Head, class... Tail>
void CodeWriter::writeToFile(Head&& head, Tail&&... tail) {
  outputfile << head << "\n";
  writeToFile(forward<Tail>(tail)...);
}

void CodeWriter::pop() { // popしてそれを現在メモリが指すものをAレジスタが保持する
  writeToFile("@SP", "M=M-1", "A=M");
}

void CodeWriter::push() { // データレジスタからstackにpush
  writeToFile("@SP", "A=M", "M=D", "@SP", "M=M+1");
}

string CodeWriter::getAOperation(int address, int bufSize) {
  char oper[bufSize];
  sprintf(oper, "@%d", address);
  return oper;
}

string CodeWriter::getAOperation(string label, int bufSize) {
  char oper[bufSize];
  const char* _label = label.c_str();
  sprintf(oper, "@%s", _label);
  return oper;
}

string CodeWriter::getLabelInFunction(string label) { // GOTOもしくはIFの中
  char cs[60];
  const char* funcName = currentFunctionName.c_str();
  const char* labelName = label.c_str();
  sprintf(cs, "%s$%s", funcName, labelName);
  return cs;
}

string CodeWriter::getLOperation(string label, int bufSize) {
  char oper[bufSize];
  const char* _label = label.c_str();
  sprintf(oper, "(%s)", _label);
  return oper;
}

label CodeWriter::getNewLabel(bool IsReturnLabel) {
  char l1[20], l2[20];
  if (IsReturnLabel) {
    returnLabelNum += 1;
    sprintf(l1, "@_RETURN_LABEL_%d", returnLabelNum);
    sprintf(l2, "(_RETURN_LABEL_%d)", returnLabelNum);
  } else {
    labelNum += 1;
    sprintf(l1, "@_IF_LABEL_%d", labelNum);
    sprintf(l2, "(_IF_LABEL_%d)", labelNum);
  }
  label ret = {l1, l2};
  return ret;
}

void CodeWriter::writeArithmetic(string command){
  if (command=="add" || command=="sub" || command=="and" || command=="or") {
    writeBinaryOperation(command);
  }
  if (command=="neg" || command=="not") {
    writeUnaryOperation(command);
  }
  if (command=="eq" || command=="gt" || command=="lt") {
    writeCompOperation(command);
  }
}

void CodeWriter::writeBinaryOperation(string command){
    // 1. stackからpopしてそれをデータメモリに格納する。
    // 2. stackからpopする。それを現在のアドレスが指すメモリに保持する。
    // 3. 計算する。
    // 4. それをpushする。
    pop();
    writeToFile("D=M");
    pop();
    if (command=="add") writeToFile("D=D+M");
    if (command=="sub") writeToFile("D=M-D");
    if (command=="and") writeToFile("D=M&D");
    if (command=="or") writeToFile("D=M|D");
    push();
}

void CodeWriter::writeUnaryOperation(string command){
  // 1. @SPの一個前のアドレスを参照
  // 2. 計算する
  string operation; 
  if (command=="neg") operation="M=-M";
  if (command=="not") operation="M=!M";
  writeToFile("@SP", "A=M-1", operation);
}

void CodeWriter::writeCompOperation(string command){
  string operation;
  if (command=="eq") operation = "D;JEQ";
  if (command=="gt") operation = "D;JGT";
  if (command=="lt") operation = "D;JLT";
  // 1. stackからpopしてそれをデータメモリに格納する。
  // 2. stackからpopする。それを現在のアドレスが指すメモリに保持する。
  // 3. そのメモリからデータメモリが指す値を引き、それらと0との大小を求める。
  // 4. true, falseで場合わけして処理する。
  pop();
  writeToFile("D=M");
  pop();
  label trueLabel = getNewLabel();
  label endLabel = getNewLabel();
  writeToFile("D=M-D", trueLabel.from, operation, "D=0", endLabel.from,
              "0;JMP",trueLabel.to, "D=-1", endLabel.to);
  push();
}

void CodeWriter::writePushPop(int command, string segment, int index){
  if (command == C_PUSH && segment=="constant") {
    string cs = getAOperation(index);
    writeToFile(cs, "D=A");
    push();
  }
  
  if (segment=="local" || segment=="argument" || segment=="this" || segment=="that") {
    string dedicatedSegement;
    if (segment == "local") dedicatedSegement = "@LCL";
    else if (segment == "argument") dedicatedSegement = "@ARG";
    else if (segment == "this") dedicatedSegement = "@THIS";
    else dedicatedSegement = "@THAT";
    if (command==C_PUSH) {
      // M[segment+index]のアドレスに格納されてる値をpushする
      writeToFile(dedicatedSegement, "A=M");
      for (int i=0; i<index; i++) writeToFile("A=A+1");
      writeToFile("D=M");
      push();
    } else {
      // POPしたものを M[segment+index]に格納
      pop();
      writeToFile("D=M", dedicatedSegement, "A=M");
      for (int i=0; i<index; i++) writeToFile("A=A+1");
      writeToFile("M=D");
    }
  }

  if (segment == "pointer" || segment == "temp") {
    int address = segment == "pointer" ? POINTER_SEGMENT: TEMP_SEGEMENT;
    string cs = getAOperation(address);
    if (command == C_PUSH) {
      writeToFile(cs);
      for (int i=0; i<index; i++) writeToFile("A=A+1");
      writeToFile("D=M");
      push();
    } else {
      pop();
      writeToFile("D=M", cs);
      for (int i=0; i<index; i++) writeToFile("A=A+1");
      writeToFile("M=D");
    }
  }

  if (segment == "static") {
    char cs[40];
    const char* cstr = vmFileName.c_str();
    sprintf(cs, "@%s.%d", cstr, index);
    if (command==C_PUSH) {
      writeToFile(cs, "D=M");
      push();
    }
    else {
      pop();
      writeToFile("D=M", cs, "M=D");
    }

  }
}

void CodeWriter::setFileName(string vmFilePath) {
  string r="";
  for (int i=vmFilePath.size()-1; i>=0; i--) {
    if (vmFilePath[i]=='/') break;
    r+=vmFilePath[i];
  }
  reverse(r.begin(), r.end());
  vmFileName = r;
}

void CodeWriter::writeLabel(string label) {
  string _label = getLabelInFunction(label);
  string cs = getLOperation(_label);
  writeToFile(cs);
}

void CodeWriter::writeGoTo(string label) {
  string _label = getLabelInFunction(label);
  string cs = getAOperation(_label);
  writeToFile(cs, "0;JMP");
}

void CodeWriter::writeIf(string label) {
  string _label = getLabelInFunction(label);
  string cs = getAOperation(_label);
  pop();
  writeToFile("D=M", cs, "D;JNE");
}

void CodeWriter::writeInit() { // 単一ファイルの場合は除外する。
  string oper = getAOperation(256);
  writeToFile(oper, "D=A", "@SP", "M=D");
  writeCall("Sys.init",0);
}

void CodeWriter::writeFunction(string functionName, int numLocals) {
  string cs = getLOperation(functionName);
  writeToFile(cs, "D=0");
  for (int i=0; i<numLocals; i++) push();
  currentFunctionName = functionName;
}

void CodeWriter::writeCall(string functionName, int numArgs) {
  label returnLabel = getNewLabel(true);
  writeToFile(returnLabel.from, "D=A");push();
  writeToFile("@LCL", "D=M");push();
  writeToFile("@ARG", "D=M");push();
  writeToFile("@THIS", "D=M");push();
  writeToFile("@THAT", "D=M");push();
  writeToFile("@SP", "D=M", "@5", "D=D-A");
  string oper = getAOperation(numArgs);
  writeToFile(oper, "D=D-A", "@ARG", "M=D");
  writeToFile("@SP", "D=M", "@LCL", "M=D");
  string operFromSymbol = getAOperation(functionName);
  writeToFile(operFromSymbol, "0;JMP", returnLabel.to);
}

void CodeWriter::writeReturn(){
  writeToFile("@LCL", "D=M", "@R13", "M=D"); // @R13を一次変数Frameとして用いる
  writeToFile("@5", "D=A", "@R13", "A=M-D", "D=M", "@R14", "M=D"); // 戻り値のアドレスを@R14に設定
  
  pop();
  writeToFile("D=M", "@ARG", "A=M", "M=D"); // *ARGにpopしたデータを代入
  writeToFile("@ARG", "D=M+1", "@SP", "M=D");  // SP=ARG+1

  writeToFile("@R13", "AM=M-1", "D=M", "@THAT", "M=D");
  writeToFile("@R13", "AM=M-1", "D=M", "@THIS", "M=D");
  writeToFile("@R13", "AM=M-1", "D=M", "@ARG", "M=D");
  writeToFile("@R13", "AM=M-1", "D=M", "@LCL", "M=D");
  writeToFile("@R14","A=M","0;JMP");
}