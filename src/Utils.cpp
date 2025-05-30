#include "Utils.h" // 包含Utils头文件
#include "UtilsString.h" // 包含UtilsString头文件
#include "Error.h" // 包含Error头文件
#include "FileReader.h" // 包含FileReader头文件
#include "FileWriter.h" // 包含FileWriter头文件
#include "Origin.h" // 包含Origin头文件
#include "PathAndDirection.h" // 包含PathAndDirection头文件
#include "ODSet.h" // 包含ODSet头文件
#include "Path.h" // 包含Path头文件
#include "StarLink.h" // 包含StarLink头文件
#include "StarNetwork.h" // 包含StarNetwork头文件
#include "PathBasedFlowMove.h" // 包含PathBasedFlowMove头文件
#include "ODMatrix.h" // 包含ODMatrix头文件
#include "PairOD.h" // 包含PairOD头文件
#include "PathSet.h" // 包含PathSet头文件

#include <cstring> // 包含C字符串处理
#include <iostream> // 包含输入输出流
#include <fstream> // 包含文件流
#include <stdio.h> // 包含标准输入输出
#include <stdlib.h> // 包含标准库函数
#include <sstream> // 包含字符串流
#include <cassert> // 包含断言
#include <math.h> // 包含数学函数

#include <boost/foreach.hpp> // 包含boost的foreach宏
#include <boost/tokenizer.hpp> // 包含boost的分词器

void Utils::getDisjointLinks(Path *path1, Path *path2, std::list<StarLink*> &list, int nbLinks){
	int indexes[nbLinks]; // 创建索引数组，大小为链接数量
	for (int i = 0; i < nbLinks; ++i) {
		indexes[i] = 0; // 初始化所有索引为0
	}
	
	for (StarLinkIterator it = path1->begin(); it != path1->end(); ++it) {
		indexes[(*it)->getIndex()] = 1; // 标记path1中的链接索引为1
	}
	int index = -1; // 初始化索引变量
	for (StarLinkIterator it = path2->begin(); it != path2->end(); ++it) {
		StarLink* link = *it; // 获取当前链接
		index = link->getIndex(); // 获取链接索引
		if (indexes[index] == 0) {
			list.push_back(link); // 如果链接不在path1中，添加到结果列表
		} else {
			indexes[index] = -1; // 标记共同链接为-1
		}
	}
	for (StarLinkIterator it = path1->begin(); it != path1->end(); ++it) {
		StarLink* link = *it; // 获取当前链接
		if (indexes[link->getIndex()] == 1) {
			list.push_back(link); // 添加path1中不在path2中的链接到结果列表
		}
	}
	assert(!list.empty()); // 断言结果列表非空
};

void Utils::getCommonLinks(PathBasedFlowMove *paths, std::list<StarLink*> &list, int nbLinks){
	int indexes[nbLinks]; // 创建索引数组，大小为链接数量
	int indexesTmp[nbLinks]; // 创建临时索引数组
	StarLink* links[nbLinks]; // 创建链接指针数组
	int size = 0; // 初始化大小变量
	for (int i = 0; i < nbLinks; ++i) {
		indexes[i] = 0; // 初始化索引数组
		indexesTmp[i] = 0; // 初始化临时索引数组
		links[i] = NULL; // 初始化链接指针数组为NULL
	}
	int index = -1; // 初始化索引变量
	Path *path = NULL; // 初始化路径指针
	int nbPaths = 0; // 初始化路径数量
	for (PathAndDirection *pathDir = paths->beginPathDirection(); pathDir != NULL; 
			pathDir = paths->getNextPathDirection()) {
		path = pathDir->getPath(); // 获取当前路径
		++nbPaths; // 增加路径计数
		for(StarLinkIterator it = path->begin(); it != path->end(); ++it){
			StarLink* link = *it; // 获取当前链接
			index = link->getIndex(); // 获取链接索引
			if (indexes[index] == 0) {
				indexesTmp[size] = index; // 存储新链接的索引
				++size; // 增加大小计数
				links[index] = link; // 存储链接指针
			}
			++indexes[index]; // 增加该链接的出现次数
		}
	}
	for (int i = 0; i < size; ++i) {
		index = indexesTmp[i]; // 获取索引
		if (indexes[index] == nbPaths) {
			list.push_back(links[index]); // 如果链接在所有路径中都出现，添加到结果列表
		}
	}
};

FPType Utils::checkFeasibility(StarNetwork *net, ODMatrix *mat){
	FPType total[net->getNbNodes()]; // 为每个节点创建总流量数组
	for (int i = 0; i < net->getNbNodes(); ++i) {
		total[i] = 0.0; // 初始化所有节点流量为0
	}
	// 加载需求
	for (OriginIterator it = mat->begin(); it != mat->end(); ++it){
		Origin* origin = *it; // 获取当前起点
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
			PairOD* dest = *jt; // 获取当前终点
			total[origin->getIndex()] += dest->getDemand(); // 从起点增加流出量
			total[dest->getIndex()] -= dest->getDemand(); // 从终点减少流入量
		}
	}
	
	// 遍历网络并检查
	for (StarLink *link = net->beginOnlyLink(); link != NULL; link = net->getNextOnlyLink()) {
		total[link->getNodeFromIndex()] -= link->getFlow(); // 减少起点节点的流量
		total[link->getNodeToIndex()] += link->getFlow(); // 增加终点节点的流量
	}
	FPType maxVal = 0.0; // 初始化最大偏差值
	for (int i = 0; i < net->getNbNodes(); ++i) {
		if (fabs(total[i]) > maxVal) maxVal = fabs(total[i]); // 找出最大偏差值
	}
	return maxVal; // 返回最大偏差值
};

FPType Utils::calculatePathDerivative(Path *path, Path *minPath, int nbLinks){
	std::list<StarLink*> list; // 创建链接列表
	Utils::getDisjointLinks(path, minPath, list, nbLinks); // 获取不相交链接
	return calculatePathDerivativeForDisjoint(list); // 计算不相交链接的导数
	
};

FPType Utils::calculatePathDerivativeForDisjoint(const std::list<StarLink*> &list){
	StarLink* link = NULL; // 初始化链接指针
	FPType der = 0.0; // 初始化导数值
	for (std::list<StarLink*>::const_iterator it = list.begin(); it != list.end(); ++it) {
		link = *it; // 获取当前链接
		der += link->getDerivative(); // 累加链接的导数
	}
	return der; // 返回总导数
};

FPType Utils::calculatePathDerivativeForDisjoint(const std::list<StarLink*> &list1,
												 const std::list<StarLink*> &list2){
	return calculatePathDerivativeForDisjoint(list1) + calculatePathDerivativeForDisjoint(list2); // 计算两个列表的导数和
};

void Utils::readART3Coordinates(const std::string& art3NodesFile, 
	std::list<Coords>& newCoords){
	FileReader read(art3NodesFile); // 创建文件读取器
	std::string line(""); // 初始化行字符串
	Coords node; // 创建坐标结构
	node.nodeID = -1; // 初始化节点ID
	node.x = -1; // 初始化x坐标
	node.y = -1; // 初始化y坐标
	int nbParams = 8; // 参数数量
	std::string params[nbParams]; // 创建参数数组
	for (int i = 0; i < nbParams; ++i) 
		params[i] = ""; // 初始化参数为空字符串
	while (read.isGood()) {
		line = read.getNextLine(); // 读取下一行
		if (!Utils::deleteWhiteSpaces(line).empty()){ // 如果行非空
			tokenizeIntoStr(line, params, nbParams); // 分词处理行
			node.nodeID = atoi(params[1].c_str()); // 设置节点ID
			node.x = atoi(params[2].c_str()); // 设置x坐标
			node.y = atoi(params[3].c_str()); // 设置y坐标
			newCoords.push_back(node); // 添加坐标到结果列表
		}
	}
};

void Utils::tokenizeIntoStr(const std::string& lineToTokenize, std::string* result, int nbTokens){
	std::stringstream ss(lineToTokenize); // 创建字符串流
	for (int i = 0; i < nbTokens; ++i) {
		ss >> result[i]; // 从字符串流读取分词到结果数组
	}
};

void Utils::mapCoordsFromART3(const std::list<Coords>& coords, StarNetwork* net, 
	std::list<Coords>& newCoords){
	Coords node; // 创建坐标结构
	node.nodeID = -1; // 初始化节点ID
	node.x = -1; // 初始化x坐标
	node.y = -1; // 初始化y坐标
	for (int i = 0; i < net->getNbNodes(); ++i){
		int id = net->findID(i); // 查找节点ID
		if (id != -1) {
			node = Utils::findCoords(coords, id); // 查找节点坐标
			if (node.nodeID == -1) {
				std::stringstream ss; // 创建字符串流
				ss << "Cannot find id = " << id; // 构建错误消息
				std::cout << ss.str() << std::endl; // 输出错误消息
			} else {
				newCoords.push_back(node); // 添加坐标到结果列表
			}
		} 
	}

};

void Utils::writeCoordsToFile(const std::list<Coords>& coords, const std::string& fileName){
	FileWriter write(fileName); // 创建文件写入器
	for (std::list<Coords>::const_iterator it = coords.begin(); it != coords.end(); ++it) {
		std::stringstream ss(""); // 创建字符串流
		ss << it->nodeID << " " << it->x << " " << it->y << "\n"; // 构建坐标行
		write.writeLine(ss.str()); // 写入行到文件
	}
};

Coords Utils::findCoords(const std::list<Coords>& coords, int nodeID) {
	for (std::list<Coords>::const_iterator it = coords.begin(); it != coords.end(); ++it){
		if (it->nodeID == nodeID) {
			return *it; // 如果找到匹配的节点ID，返回坐标
		}
	}
	Coords emptyNode; // 创建空坐标结构
	emptyNode.nodeID = -1; // 设置节点ID为-1表示未找到
	emptyNode.x = -1; // 设置x坐标为-1
	emptyNode.y = -1; // 设置y坐标为-1
	return emptyNode; // 返回空坐标结构
};

FPType Utils::generateRndNumber(FPType maxVal){
	return static_cast <FPType> (rand()) / (static_cast <FPType> (RAND_MAX/maxVal)); // 生成[0,maxVal]范围内的随机浮点数
}
