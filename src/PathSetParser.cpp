#include "PathSetParser.h"     // 包含路径集合解析器头文件
#include "FileReader.h"         // 包含文件读取器头文件
#include "UtilsString.h"        // 包含字符串工具头文件
#include "PathSet.h"            // 包含路径集合头文件
#include "StarNetwork.h"        // 包含星型网络头文件
#include "Path.h"               // 包含路径头文件
#include "ODSet.h"              // 包含OD集合头文件

#include <boost/foreach.hpp>    // 包含boost遍历宏
#include <boost/tokenizer.hpp>  // 包含boost标记解析器

// OD对信息结构体
struct ODPairInfo {
	int index;       // OD对索引
	int originIndex; // 起点索引
	int destIndex;   // 终点索引
	int nbPaths;     // 路径数量
};

// 打印OD对信息
// info: OD对信息结构体引用
void printODPairInfo(const ODPairInfo& info) {
	std::cout << "index = " << info.index << " originIndex = " << info.originIndex <<
		" destIndex = " << info.destIndex << " nbPaths = " << info.nbPaths << std::endl; // 输出OD对信息
};

// 初始化OD对信息
// info: OD对信息结构体引用
void initialiseODPairInfo(ODPairInfo& info) {
	info.index = -1;       // 初始化索引为-1
	info.originIndex = -1; // 初始化起点索引为-1
	info.destIndex = -1;   // 初始化终点索引为-1
	info.nbPaths = -1;     // 初始化路径数量为-1
};

// 从字符串中获取下一个索引值
// line: 输入字符串
// indexName: 索引名称(可选)
// 返回值: 提取的索引值字符串
std::string getNextIndex(const std::string& line, const std::string& indexName = "") {
	size_t indexPos = 0;                           // 索引位置
	if (indexName != "") indexPos = line.find(indexName); // 如果提供了索引名称，则查找位置
	assert(indexPos != std::string::npos);         // 断言找到了索引位置
	std::string subLine = line.substr(indexPos);   // 获取子字符串
	std::string index = Utils::getSubString("{", "}", subLine); // 提取花括号内的内容
	assert(index != "");                           // 断言索引不为空
	return index;                                  // 返回索引字符串
};

// 解析头部信息
// line: 输入字符串
// 返回值: 解析后的OD对信息结构体
ODPairInfo parseHeader(const std::string& line) {
	ODPairInfo info;                              // 创建OD对信息结构体
	initialiseODPairInfo(info);                   // 初始化OD对信息
	info.index = atoi(getNextIndex(line, "Path set").c_str()); // 解析OD对索引
	info.originIndex = atoi(getNextIndex(line, "originIndex").c_str()); // 解析起点索引
	info.destIndex = atoi(getNextIndex(line, "destIndex").c_str()); // 解析终点索引
	info.nbPaths = atoi(getNextIndex(line, "nbPaths").c_str()); // 解析路径数量
	return info;                                 // 返回OD对信息
};

// 创建路径
// line: 输入字符串
// net: 星型网络引用
// 返回值: 创建的路径指针
Path* createPath(const std::string& line, const StarNetwork& net) {
	std::string links = Utils::getSubString("{", "}", line); // 提取花括号内的链路字符串
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer; // 定义标记类型
    boost::char_separator<char> sep(" ");                   // 定义空格分隔符
    tokenizer tokens(links, sep);                           // 创建标记解析器

    Path* path = new Path;                                  // 创建新路径
    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter){ // 遍历所有标记
    	std::string token = *tok_iter;                      // 获取当前标记
    	path->pushBackLink(net.getLink(atoi(token.c_str()))); // 添加链路到路径
    }
    return path;                                           // 返回创建的路径
};

// 获取浮点值
// line: 输入字符串
// 返回值: 解析后的浮点数
FPType getFloatValue(const std::string& line) {
	return strtold(getNextIndex(line).c_str(), NULL);      // 将字符串转换为长双精度浮点数
};

// 解析路径集合
// paths: 路径集合引用
// net: 星型网络引用
// fileName: 文件名
void PathSetParser::parse(PathSet& paths, const StarNetwork& net, const std::string& fileName) {
	
	FileReader reader(fileName);                           // 创建文件读取器
	ODPairInfo info;                                      // 创建OD对信息结构体
	initialiseODPairInfo(info);                           // 初始化OD对信息
	Path* path = NULL;                                    // 初始化路径指针为NULL
	while (reader.isGood()) {                             // 当文件读取良好时循环
		std::string line = reader.getNextLine();          // 读取下一行
		if (line.find("Path set") != std::string::npos) { // 如果行包含"Path set"
			info = parseHeader(line);                     // 解析头部信息
			path = NULL;                                  // 重置路径指针
		} else if (line.find("Path") != std::string::npos) { // 如果行包含"Path"
			assert(info.index != -1);                     // 断言索引有效
			ODSet* odset = paths.getODSetByIndex(info.index); // 获取OD集合
			assert(odset->getOriginIndex() == info.originIndex &&
				odset->getDestIndex() == info.destIndex); // 断言起点和终点索引匹配
			path = createPath(line, net);                 // 创建路径
			odset->addPath(path);                         // 将路径添加到OD集合
		} else if (line.find("flow") != std::string::npos) { // 如果行包含"flow"
			assert(path != NULL);                         // 断言路径指针不为NULL
			path->setFlow(getFloatValue(line));           // 设置路径流量
		} else if (line.find("time") != std::string::npos) { // 如果行包含"time"
			assert(path != NULL);                         // 断言路径指针不为NULL
			path->setCurrCost(getFloatValue(line));       // 设置路径当前成本
		} 
	}
};