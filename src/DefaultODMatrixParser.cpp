// 包含必要的头文件
#include "DefaultODMatrixParser.h"
#include "Error.h"
#include "FileReader.h"
#include "UtilsString.h"
#include "Origin.h"
#include "PairOD.h"
#include "ODMatrix.h"
#include "StarNetwork.h"

#include <cassert>
#include <stdlib.h>

// 构造函数，初始化OD矩阵文件路径
DefaultODMatrixParser::DefaultODMatrixParser(const std::string& fileWithODMatrix) : 
		fileWithODMatrix_(fileWithODMatrix) {

};

// 析构函数
DefaultODMatrixParser::~DefaultODMatrixParser(){

};

// 解析OD矩阵文件并创建OD矩阵对象
ODMatrix* DefaultODMatrixParser::parseODMatrix(StarNetwork* net){
	FileReader readFile(fileWithODMatrix_); // 创建文件读取器
	int nbNodes = net->getNbNodes(); // 获取网络节点数量
	// 跳过元数据部分
	std::string line = "";
	while (line.find("<END OF METADATA>") == std::string::npos) {
		line = readFile.getNextLine(); 
		
		if (!readFile.isGood()){ 
			std::string message = "<END OF METADATA> 在文件中缺失: ";
			message.append(fileWithODMatrix_);
			Error er(message);
			throw er;
		}
	}
	size_t found;
	int originID = -1; // 起点ID
	int destID = -1; // 终点ID
	FPType demand = 0.0; // 需求量
	Origin *origin = NULL; // 起点对象指针
	std::list<Origin*> allOrigins; // 存储所有起点的列表
	std::string newLine = "";
	while (readFile.isGood()) { 
		line = readFile.getNextLine(); // 读取下一行
		line = Utils::skipOneLineComment("~", line); // 跳过注释行
		if (!Utils::deleteWhiteSpaces(line).empty()){
			// 检查是否是新起点
			found = line.find("Origin");
			if (found != std::string::npos) {
				originID = atoi((line.substr(found + 7)).c_str()); // 提取起点ID
				origin = new Origin(net->getNodeIndex(originID)); // 创建新起点
				allOrigins.push_back(origin);	// 添加到起点列表
			}
			assert(origin != NULL);
			// 从当前行获取所有目的地
			size_t pos = extractDestination(0, line, destID, demand);
			
			while (pos != std::string::npos){
				if ((destID != originID) && (demand != 0.0)) {
					origin->addDestination(new PairOD(net->getNodeIndex(destID),
											demand)); // 添加OD对
				}
				pos = extractDestination(pos, line, destID, demand);
			}
		}
	}
	ODMatrix *mat = new ODMatrix(nbNodes, allOrigins.size()); // 创建OD矩阵
	for (std::list<Origin*>::iterator it = allOrigins.begin();
			it != allOrigins.end(); ++it) {
		mat->addOrigin(*it); // 添加所有起点到矩阵
	}
	mat->setIndexes(); // 设置索引
	return mat;
};

// 从字符串中提取目的地信息
size_t DefaultODMatrixParser::extractDestination(size_t posFrom, const std::string& line, 
			int &destID, FPType &demand) const{
	size_t pos1 = line.find(":", posFrom); // 查找冒号位置
	if (pos1 != std::string::npos) {
		size_t pos2 = line.find(";", pos1); // 查找分号位置
		if (pos2 != std::string::npos) {
			destID = atoi((line.substr(posFrom, pos1 - posFrom - 1)).c_str()); // 提取目的地ID
			demand = atof((line.substr(pos1 + 1, pos2 - pos1 - 1)).c_str()); // 提取需求量
			return pos2 + 1; // 返回下一个位置
		}
		return std::string::npos;
	}
	return std::string::npos;
};