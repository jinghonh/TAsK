// 包含必要的头文件
#include "FileWriter.h"
#include "Error.h"

// 构造函数：初始化文件写入器，可选择是否追加模式
FileWriter::FileWriter(const std::string& fileName, bool append) : name_(fileName) { 
    // 根据append参数决定打开模式
    if (append) {
        // 以追加模式打开文件
        file_.open(fileName.c_str(),  std::ios::out | std::ios::app);
    } else {
        // 以覆盖模式打开文件
        file_.open(fileName.c_str(),  std::ios::out | std::ios::trunc);
    }
    // 检查文件是否成功打开
    if (!file_.is_open()) {
        std::string message("无法创建文件: ");
        message.append(fileName);
        throw Error(message);
    }
};

// 析构函数：关闭文件
FileWriter::~FileWriter(){
    file_.close();
};

// 写入一行内容到文件
void FileWriter::writeLine(const std::string& line){
    file_ << line;
};

// 添加一个空行
void FileWriter::addEmptyLine(){
    file_ << "\n";
};

// 获取文件名
std::string FileWriter::getName() const{
    return name_;
};
