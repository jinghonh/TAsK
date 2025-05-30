// 包含必要的头文件
#include "NetOutput.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "UtilsString.h"
#include "Error.h"
#include "StarNetwork.h"

#include <sstream>

// 构造函数：初始化网络和缺失ID的日志文件
NetOutput::NetOutput(StarNetwork* net, const std::string& logFileForMissingIDs) : net_(net),
			logFileForMissingIDs_(logFileForMissingIDs) {

};

// 析构函数
NetOutput::~NetOutput(){

};

// 读取坐标文件的方法
void NetOutput::readCoord(const std::string& file, std::vector<FPType>& xCoord, 
				std::vector<FPType>& yCoord, std::vector<int>& nodeID){
	// 创建文件读取器
	FileReader fileCoords(file);
	// 跳过第一行
	std::string temp = fileCoords.getNextLine(); 
	// 标记是否有ID缺失
	bool someIDMissing = false;
	// 创建日志文件写入器
	FileWriter writeMissingIDs(logFileForMissingIDs_);
	// 循环读取文件内容
	while (fileCoords.isGood()){
		// 读取下一行
		std::string line = fileCoords.getNextLine();
		// 检查行是否为空
		if (!Utils::deleteWhiteSpaces(line).empty()) {
			// 声明变量存储节点ID和坐标
			int nodeIDval;
			FPType x;
			FPType y;
			// 创建字符串流解析行内容
			std::stringstream lineToTokenize(line);
			// 解析节点ID和坐标值
			lineToTokenize >> nodeIDval >> x >> y >> temp;
			try {
				// 获取节点索引
				int nodeIndex = net_->getNodeIndex(nodeIDval);
				// 存储节点信息
				nodeID[nodeIndex] = nodeIDval;
				xCoord[nodeIndex] = x; 
				yCoord[nodeIndex] = y; 		
			} catch (Error e) {
				// 处理缺失ID的情况
				someIDMissing = true;
				writeMissingIDs.writeLine(e.getMessage() + "\n");
			}
		}
	}
	// 如果有缺失的ID，输出提示信息
	if (someIDMissing) {
		std::cout << "Some node IDs are missing. See " << logFileForMissingIDs_ << std::endl;
	}
};
