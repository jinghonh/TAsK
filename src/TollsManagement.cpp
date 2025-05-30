#include "TollsManagement.h" // 包含收费管理头文件
#include "Utils.h" // 包含工具类头文件
#include "UtilsString.h" // 包含字符串工具类头文件
#include "StarNetwork.h" // 包含星形网络头文件
#include "Origin.h" // 包含起点头文件
#include "FileWriter.h" // 包含文件写入器头文件
#include "FileReader.h" // 包含文件读取器头文件
#include "ShortestPath.h" // 包含最短路径头文件
#include "ODMatrix.h" // 包含OD矩阵头文件
#include "PairOD.h" // 包含OD对头文件

#include <sstream> // 包含字符串流头文件
#include <cassert> // 包含断言头文件
#include <stdlib.h> // 包含标准库头文件

FPType getRndNumberWithProbability(TollType val, 
		FPType probabylity, TollType maxToll){ // 根据概率获取随机数的函数
	if (Utils::generateRndNumber(1.0) <= probabylity) { // 如果生成的随机数小于等于概率值
			return rand() % maxToll + 1; // 返回1到maxToll之间的随机数
	}
	return val; // 否则返回原值
};

void assignRndTolls(ShortestPath* shPath, int destIndex, TollContainerType& tolls, 
		FPType probabylity, TollType maxToll){ // 分配随机收费的函数
	StarLink *link = shPath->getInComeLink(destIndex); // 获取到达目的地的入边
	int nextDest = link->getNodeFromIndex(); // 获取链接的起始节点索引
	int linkIndex = -1; // 初始化链接索引
	while (link != NULL) { // 当链接不为空时
		linkIndex = link->getIndex(); // 获取链接索引
		tolls[linkIndex] = getRndNumberWithProbability(tolls[linkIndex], probabylity, maxToll); // 根据概率更新收费
		nextDest = link->getNodeFromIndex(); // 获取链接的起始节点索引
		link = shPath->getInComeLink(nextDest); // 获取到达起始节点的入边
	}
};

bool isHeaderLine(const std::string& line){ // 判断是否为头部行的函数
	return line.find("<") != std::string::npos; // 如果行中包含"<"则返回true
};

bool isCommentLine(const std::string& line) { // 判断是否为注释行的函数
	return line.find("~") != std::string::npos; // 如果行中包含"~"则返回true
};
void createHeader(FileWriter& writer, FileReader& reader) { // 创建头部的函数
	std::string line(""); // 初始化行字符串
	while (true){ // 无限循环
		line = reader.getNextLine() + "\n"; // 获取下一行并添加换行符
		if (isHeaderLine(line)) { // 如果是头部行
			writer.writeLine(line); // 写入文件
		} else if (isCommentLine(line)) { // 如果是注释行
			writer.writeLine(line); // 写入文件
			return; // 返回
		}
		if (!reader.isGood()) return; // 如果读取出错，返回
	}
};

int findLinkIndex(int tail, int head, StarNetwork* net, int guessIndex){ // 查找链接索引的函数
	StarLink* link = net->getLink(guessIndex); // 获取猜测索引的链接
	assert(link != NULL); // 断言链接不为空
	if (link->getNodeFrom() == tail && link->getNodeTo() == head) { // 如果链接的起点和终点匹配
		return link->getIndex(); // 返回链接索引
	}
	return -1; // 否则返回-1
};

void writeLinkToFile(FileWriter& writer, const std::string& line, 
	const TollContainerType& tolls, StarNetwork* net, int guessIndex) { // 将链接写入文件的函数
	std::stringstream ss(line); // 创建字符串流
	int tail = -1; // 初始化起点
	int head = -1; // 初始化终点
	std::string capacity(""); // 初始化容量字符串
	std::string length(""); // 初始化长度字符串
	std::string freeFlow(""); // 初始化自由流字符串
	std::string b(""); // 初始化b字符串
	std::string power(""); // 初始化幂字符串
	std::string speed(""); // 初始化速度字符串
	std::string toll(""); // 初始化收费字符串
	std::string type(""); // 初始化类型字符串
	ss >> tail >> head >> capacity >> length >> freeFlow >> b >> power >> speed >> toll >> type; // 从字符串流中读取各个字段
	int linkIndex = findLinkIndex(tail, head, net, guessIndex); // 查找链接索引
	assert(linkIndex != -1); // 断言链接索引有效
	ss.str(""); // 清空字符串流
	ss << tail << "\t" << head << "\t" << capacity << "\t" << length << "\t" <<
		freeFlow << "\t" << b << "\t" << power << "\t" << speed << "\t" << tolls[linkIndex]
		<< "\t" << type << "\t;" << "\n"; // 构造新的行字符串
	writer.writeLine(ss.str()); // 写入文件
};



void TollsManagement::generateRandomTolls(TollContainerType& tolls, 
		FPType probabylity, TollType maxToll){ // 生成随机收费的方法
	int size = tolls.size(); // 获取收费容器大小
	for (int i = 0; i < size; ++i) { // 遍历所有收费
		tolls[i] = getRndNumberWithProbability(tolls[i], probabylity, maxToll); // 根据概率更新收费
	}
};

void TollsManagement::generateRandomTollsOnShortestPath(TollContainerType& tolls, 
		FPType probabylity, TollType maxToll, ODMatrix* mat, ShortestPath *shPath){ // 在最短路径上生成随机收费的方法
	for (OriginIterator it = mat->begin(); it != mat->end(); ++it) { // 遍历所有起点
		Origin* origin = *it; // 获取当前起点
		shPath->calculate(origin->getIndex()); // 计算从当前起点出发的最短路径
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) { // 遍历当前起点的所有终点
			if (Utils::generateRndNumber(1.0) <= probabylity) { // 如果生成的随机数小于等于概率值
				PairOD* dest = *jt; // 获取当前终点
				assignRndTolls(shPath, dest->getIndex(), tolls, probabylity, maxToll); // 分配随机收费
			}
		}
		
	}
};

void TollsManagement::writeTollsToFile(const TollContainerType& tolls, const std::string& existingFile, 
		const std::string& newFile, StarNetwork* net){ // 将收费写入文件的方法
	FileWriter writer(newFile); // 创建文件写入器
	FileReader reader(existingFile); // 创建文件读取器

	createHeader(writer, reader); // 创建头部

	std::string line(""); // 初始化行字符串
	int guessIndex = 0; // 初始化猜测索引
	while (reader.isGood()) { // 当读取正常时
		line = reader.getNextLine(); // 获取下一行
		if (!Utils::deleteWhiteSpaces(line).empty()) { // 如果删除空白后行不为空
			writeLinkToFile(writer, line, tolls, net, guessIndex); // 将链接写入文件
			++guessIndex; // 增加猜测索引
		}
	}
	
};
