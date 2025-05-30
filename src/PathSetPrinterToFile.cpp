#include "PathSetPrinterToFile.h" // 包含头文件
#include "FileWriter.h" // 包含文件写入器类
#include "Path.h" // 包含路径类
#include "ODSet.h" // 包含OD集合类
#include "PathSet.h" // 包含路径集合类
#include "ODMatrix.h" // 包含OD矩阵类

#include <sstream> // 包含字符串流
#include <iomanip> // 包含输入输出格式控制

PathSetPrinterToFile::PathSetPrinterToFile(ODMatrix* mat):
		mat_(mat) { // 构造函数，初始化OD矩阵

};

PathSetPrinterToFile::~PathSetPrinterToFile(){ // 析构函数

};

void PathSetPrinterToFile::writePathSetToFile(PathSet* pathSet, const std::string& fileName) const{ // 将路径集合写入文件的方法
	FileWriter writer(fileName); // 创建文件写入器
	for (ConstODSetIterator odIt = pathSet->begin(); odIt != pathSet->end(); ++odIt){ // 遍历所有OD集合
		ODSet* od = *odIt; // 获取OD集合
		writer.writeLine(createStringForOD(od, od->getIndex())); // 写入OD集合信息
		long int pathCount = 0; // 初始化路径计数
		for (PathIterator pathIt = od->begin(); pathIt != od->end(); ++pathIt) { // 遍历所有路径
			writer.writeLine(createStringForPath(*pathIt, pathCount)); // 写入路径信息
			++pathCount; // 路径计数递增
		}
	}
};

std::string PathSetPrinterToFile::createStringForOD(ODSet* od, long int index) const { // 为OD对创建字符串的方法
	int originIndex = od->getOriginIndex(); // 获取起点索引
	int destIndex = od->getDestIndex(); // 获取目的地索引
	std::stringstream ss; // 创建字符串流
	ss << "Path set {" << index << "}: originIndex = {" << originIndex << 
		"}, destIndex = {" << destIndex << "}, demand = {" << mat_->getDemandByIndex(originIndex,
																					 destIndex)
		<< "}, nbPaths = {" << od->getNbPaths() << "}; \n"; // 格式化输出OD信息
	return ss.str(); // 返回字符串
};

std::string PathSetPrinterToFile::createStringForPath(Path* path, long int index) const { // 为路径创建字符串的方法
	std::stringstream ss; // 创建字符串流
	ss << "Path #" << index << ": {"; // 开始格式化路径信息
	int linkIndex = -1; // 初始化链接索引
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) { // 遍历路径中所有链接
		linkIndex = (*it)->getIndex(); // 获取链接索引
		ss << linkIndex << " "; // 添加链接索引到输出
	}
	ss << "}"; // 结束链接列表
	ss << std::setprecision(22) << "\n" << "\t flow = {" << path->getFlow() << "}\n\t time = {" << 
		path->getCurrCost() << "}\n"; // 添加流量和时间信息
	return ss.str(); // 返回字符串
}; 