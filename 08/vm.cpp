#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm> // std::equal
#include <iterator>  // std::rbegin, std::rend
#include "parser.hpp"
#include "code_writer.hpp"
#include "constants.hpp"
using namespace std;

/**
* @brief 文字列の語尾がsuffixと一致するかの検証関数
* @param[in]    s  ターゲット文字列
* @param[out]   suffix  語尾の文字列
* @return        true:成功, false:失敗
*/
bool ends_with(const std::string& s, const std::string& suffix) {
   if (s.size() < suffix.size()) return false;
   return std::equal(std::rbegin(suffix), std::rend(suffix), std::rbegin(s));
}

/**
* @brief フォルダ以下のファイル一覧を取得する関数
* @param[in]    folderPath  フォルダパス
* @param[out]   file_names  ファイル名一覧
* @return        true:成功, false:失敗
*/
bool getFileNames(std::string folderPath, std::vector<std::string> &file_names)
{
  filesystem::directory_iterator iter(folderPath), end;
  error_code err;

  for (; iter != end && !err; iter.increment(err)) {
    const filesystem::directory_entry entry = *iter;
    string fileName = entry.path().string(); 
    if (ends_with(fileName, ".vm")) {
      file_names.push_back(fileName);
      printf("%s\n", file_names.back().c_str());
    }
  }

  /* エラー処理 */
  if (err) {
    cout << err.value() << std::endl;
    cout << err.message() << std::endl;
    return false;
  }
  return true;
}

string getOutputFileName(string path, bool isFile) {
  string outputFileName="";
  if (isFile) {
    for (char c: path) {
      if (c=='.') break;
      outputFileName+=c;
    }
  } else {
    string::size_type pos = path.rfind("/", path.size()-1);
    string fileName =  path.substr(pos+1);
    outputFileName = path + '/' +fileName;
  }
  outputFileName = outputFileName + ".asm";
  return outputFileName;
}

void writeFile(string fileName, string outputFileName, bool writeInit=false) {
  ifstream ifs(fileName, ios::in);
  if(!ifs){
    cerr << "Error: file not opened." << "\n";
    return;
  }
  CodeWriter Cw(outputFileName);
  Cw.setFileName(outputFileName);
  if (writeInit) Cw.writeInit();
  Parser Parser(fileName);
  while(Parser.advance()) {
    int cType = Parser.commandType();
    if (cType==0) continue;
    if (cType == C_RETURN) {
      Cw.writeReturn();
      continue;
    }
    if (cType == C_ARITHMETIC) {
      string command = Parser.arg1();
      Cw.writeArithmetic(command);
      continue;
    }
    if (cType == C_LABEL) {
      string label = Parser.arg1();
      Cw.writeLabel(label);
      continue;
    } else if (cType == C_GOTO) {
      string label = Parser.arg1();
      Cw.writeGoTo(label);
      continue;
    } else if (cType == C_IF) {
      string label = Parser.arg1();
      Cw.writeIf(label);
      continue;
    }
    
    string segment = Parser.arg1();
    int index = Parser.arg2();
    if (cType==C_POP || cType==C_PUSH) {
      Cw.writePushPop(cType, segment, index);
    } else if (cType == C_FUNCTION) {
      Cw.writeFunction(segment, index);
    } else if (cType == C_CALL) {
      Cw.writeCall(segment, index);
    }
  }
}

int main(int argc, char* argv[]) {
  string path = argv[1];
  bool isFile = ends_with(path, ".vm");
  string outputFileName = getOutputFileName(path, isFile);
  ifstream ofs(outputFileName, ios::in);
  if(ofs) filesystem::remove(outputFileName);
  if (isFile) writeFile(path, outputFileName);
  else {
    // target: directory
    vector<string> fileNames;
    if (getFileNames(path, fileNames)) {
      for (int i=0; i<fileNames.size(); i++) {
        if (i==0) writeFile(fileNames[i], outputFileName, true);
        else writeFile(fileNames[i], outputFileName);
      }
    }
  }
  return 0;
}