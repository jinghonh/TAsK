#include "ParseIndiffCurves.h" // 包含无差异曲线解析器头文件
#include "FileReader.h" // 包含文件读取器头文件
#include "PieceWiseLinearIndiffCurve.h" // 包含分段线性无差异曲线头文件
#include "UtilsString.h" // 包含字符串工具类头文件
#include "Error.h" // 包含错误处理类头文件
#include "IndiffCurveContainer.h" // 包含无差异曲线容器头文件

#include <sstream> // 包含字符串流头文件
#include <cassert> // 包含断言头文件

ParseIndiffCurves::ParseIndiffCurves() { // 构造函数实现

};

ParseIndiffCurves::~ParseIndiffCurves() { // 析构函数实现

};

int ParseIndiffCurves::getODIndex(const std::string& line) { // 从字符串中获取OD对索引方法实现
	size_t pos = line.find("("); // 查找左括号位置
	assert(pos != std::string::npos); // 断言找到了左括号
	std::string first3numbers = line.substr(0, pos - 1); // 提取前三个数字（减1是为了删除空格）
	int originIndex = -1; // 初始化起点索引
	int destIndex = -1; // 初始化终点索引
	int odIndex = -1; // 初始化OD对索引
	std::stringstream ss(first3numbers); // 创建字符串流
	ss >> originIndex >> destIndex >> odIndex; // 从字符串流中读取三个索引
	assert(odIndex != -1); // 断言OD对索引有效
	return odIndex; // 返回OD对索引
};

FPType ParseIndiffCurves::getFirst(const std::string& line) { // 获取第一个值（收费）方法实现
	size_t pos = line.find(","); // 查找逗号位置
	assert(pos != std::string::npos); // 断言找到了逗号
	return strtold(line.substr(0, pos).c_str(), NULL); // 将逗号前的子字符串转换为浮点数并返回
};

FPType ParseIndiffCurves::getSecond(const std::string& line)  { // 获取第二个值（旅行时间）方法实现
	size_t pos = line.find(","); // 查找逗号位置
	assert(pos != std::string::npos); // 断言找到了逗号
	return strtold(line.substr(pos + 1).c_str(), NULL); // 将逗号后的子字符串转换为浮点数并返回
};

int ParseIndiffCurves::getCurvePoints(const std::string& line, PointsContainer& points) { // 从字符串中获取曲线点方法实现
	int count = 0; // 初始化点计数器
	std::string tmpLine(line); // 创建字符串副本
	while (true) { // 无限循环直到捕获异常
		try {
			std::string substr = Utils::getSubString("(", ")", tmpLine); // 获取括号内的子字符串
			assert(!substr.empty()); // 断言子字符串不为空
			points.push_back(std::make_pair<FPType, FPType>(getFirst(substr), getSecond(substr))); // 创建点并添加到容器
			++count; // 增加点计数
			tmpLine = tmpLine.substr(tmpLine.find(")") + 1); // 更新临时字符串，继续查找下一个点
		} catch (Error e) {
			return count; // 如果捕获到异常，返回已处理的点数
		}
	}
	return count; // 返回点数（实际上不会执行到这里）
};

IndiffCurve* ParseIndiffCurves::createCurve(int nbPoints, const PointsContainer& points,
	const std::string& curveType) { // 创建曲线方法实现
	assert(curveType == ""); // 断言曲线类型为空（基类只处理默认的分段线性曲线）
	PieceWiseLinearIndiffCurve* curve = new PieceWiseLinearIndiffCurve(nbPoints); // 创建分段线性无差异曲线
	for (PointsContainer::const_iterator it = points.begin(); it != points.end(); ++it) { // 遍历所有点
		curve->addPoint(it->first, it->second); // 将点添加到曲线
	}
	return curve; // 返回创建的曲线
};

std::string ParseIndiffCurves::getCurveType(const std::string& line) { // 获取曲线类型方法实现
	if (line.find("Linear") != std::string::npos) { // 如果找到"Linear"关键字
		return "Linear"; // 返回"Linear"
	} else if (line.find("Convex") != std::string::npos) { // 如果找到"Convex"关键字
		return "Convex"; // 返回"Convex"
	} else if (line.find("Concave") != std::string::npos) { // 如果找到"Concave"关键字
		return "Concave"; // 返回"Concave"
	}
	return ""; // 如果未找到以上任何关键字，返回空字符串
};

void ParseIndiffCurves::parse(const std::string& fileName, IndiffCurveContainer& container) { // 解析文件方法实现
	FileReader reader(fileName); // 创建文件读取器

	while (reader.isGood()) { // 当文件读取状态良好时循环
		std::string line(reader.getNextLine()); // 获取下一行
		if (!Utils::skipOneLineComment("//", line).empty()) { // 如果不是注释行
			int odIndex = getODIndex(line); // 获取OD对索引
			PointsContainer points; // 创建点容器
			int nbPoints = getCurvePoints(line, points); // 获取曲线点
			container.addCurve(odIndex, createCurve(nbPoints, points, getCurveType(line))); // 创建曲线并添加到容器
		}
	}
};