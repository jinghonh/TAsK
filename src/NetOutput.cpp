// 包含必要的头文件
#include "NetOutput.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "UtilsString.h"
#include "Error.h"
#include "StarNetwork.h"

#include <sstream>
#include <limits>

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
	
	// 归一化坐标，将经纬度值映射到更大的范围以便于显示
	// 找出x和y坐标的最小值和最大值
	FPType xMin = std::numeric_limits<FPType>::max();
	FPType xMax = std::numeric_limits<FPType>::lowest();
	FPType yMin = std::numeric_limits<FPType>::max();
	FPType yMax = std::numeric_limits<FPType>::lowest();
	
	for (size_t i = 0; i < xCoord.size(); ++i) {
		// 使用-1作为无效坐标的标记
		if (xCoord[i] != -1 && yCoord[i] != -1) { 
			xMin = std::min(xMin, xCoord[i]);
			xMax = std::max(xMax, xCoord[i]);
			yMin = std::min(yMin, yCoord[i]);
			yMax = std::max(yMax, yCoord[i]);
		}
	}
	
	// 将坐标映射到[0,2]的范围
	const FPType targetRange = 2.0;
	for (size_t i = 0; i < xCoord.size(); ++i) {
		// 使用-1作为无效坐标的标记
		if (xCoord[i] != -1 && yCoord[i] != -1) {
			// 归一化到[0,1]然后缩放到目标范围
			xCoord[i] = ((xCoord[i] - xMin) / (xMax - xMin)) * targetRange;
			yCoord[i] = ((yCoord[i] - yMin) / (yMax - yMin)) * targetRange;
		}
	}
	
	// 如果有缺失的ID，输出提示信息
	if (someIDMissing) {
		std::cout << "Some node IDs are missing. See " << logFileForMissingIDs_ << std::endl;
	}
};
