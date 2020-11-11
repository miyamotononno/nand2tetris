#include <iostream>
#include <fstream>
#include <filesystem>
#include <queue>
#include "compilation_engine.hpp"
#include "jack_tokenizer.hpp"
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
    if (ends_with(fileName, ".jack")) {
      file_names.push_back(fileName);
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

/**
* @brief 出力するファイル名を取得する関数
* @param[in]    inputFileName  入力ファイル名
* @return       出力ファイル名
*/

string getOutputFileName(string inputFileName) {
  string outputFileName="";
  for (char c: inputFileName) {
    if (c=='.') break;
    outputFileName+=c;
  }
  outputFileName = outputFileName + ".vm";
  return outputFileName;
}


int main(int argc, char* argv[]) {
  string path = argv[1];
  vector<string> fileNames;
  if (getFileNames(path, fileNames)) {
    for (auto fileName: fileNames) {
      ifstream ifs(fileName, ios::in);
      if(!ifs){
        cerr << "Error: file not opened." << "\n";
        return 1;
      }
      JackTokenizer JT(&ifs);
      queue<P> terminalSymbolQue;
      JT.advance(terminalSymbolQue);
      string outputFileName = getOutputFileName(fileName);
      ifstream ofs(outputFileName, ios::in);
      if(ofs) filesystem::remove(outputFileName);
      cout << "========== "<< outputFileName << "========== "<< endl;
      CompilationEngine ce(terminalSymbolQue, outputFileName);
      ce.compileClass();
    }
  }
  return 0;
}
