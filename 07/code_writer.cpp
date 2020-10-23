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

label CodeWriter::getNewLabel() {
  label_num += 1;
  char l1[20], l2[20];
  sprintf(l1, "@LABEL%d", label_num);
  sprintf(l2, "(LABEL%d)", label_num);
  label ret = {l1, l2};
  return ret;
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
    char cs[20];
    sprintf(cs, "@%d", index);
    writeToFile(cs, "D=A");
    push();
  }
  
  if (segment=="local" || segment=="argument" || segment=="this" || segment=="that") {
    string dedicatedSegement;
    if (segment == "local") dedicatedSegement = "@LCL";
    else if (segment == "argument") dedicatedSegement = "@ARG";
    else if (segment == "this") dedicatedSegement = "@THIS";
    else dedicatedSegement = "@THAT";
    if (command==C_PUSH) writePushFromDedicatedSegment(dedicatedSegement, index);
    else writePopToDedicatedSegment(dedicatedSegement, index);
  }

  if (segment == "pointer" || segment == "temp") {
    int address = segment == "pointer" ? POINTER_SEGMENT: TEMP_SEGEMENT;
    char cs[20];
    sprintf(cs, "@%d", address);
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

void CodeWriter::writePopToDedicatedSegment(string segment, int index) {
  // POPしたものを M[segment+index]に格納
  pop();
  writeToFile("D=M", segment, "A=M");
  for (int i=0; i<index; i++) writeToFile("A=A+1");
  writeToFile("M=D");
}

void CodeWriter::writePushFromDedicatedSegment(string segment, int index) {
  // M[segment+index]のアドレスに格納されてる値をpushする
  writeToFile(segment, "A=M");
  for (int i=0; i<index; i++) writeToFile("A=A+1");
  writeToFile("D=M");
  push();
}

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