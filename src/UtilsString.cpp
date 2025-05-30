#include "UtilsString.h" // 包含UtilsString头文件
#include "Error.h" // 包含Error头文件

#include <cstring> // 包含C字符串处理
#include <algorithm> // 包含算法库
#include <stdlib.h> // 包含标准库函数

void Utils::checkEmptyValue(int value){
	if (value == 0) {
		std::string message = "Some values of attributes are missing"; // 属性值缺失的错误消息
		Error er(message); // 创建错误对象
		throw er; // 抛出错误
	}
};

FPType Utils::extractNumber(const std::string& line, size_t pos, size_t &nextPos){
	int len = line.length(); // 获取字符串长度
	int i = pos; // 从指定位置开始
	char ch; // 存储当前字符
	bool dotAdded = false; // 标记是否已添加小数点
	for (; i < len; ++i) {
		ch = line.at(i); // 获取当前字符
		if (isdigit(ch) || (ch == '.')) {
			if (ch == '.') dotAdded = true; // 如果是小数点，标记为已添加
			break; // 找到数字或小数点，退出循环
		}
	}
	if (i == len) throw Error("Current line does not contain number."); // 如果到达行尾仍未找到数字，抛出错误
	std::string num(1, line.at(i)); // 将第一个数字或小数点添加到结果字符串
	
	int j = i + 1; // 从下一个字符开始
	for (; j < len; ++j) {
		ch = line.at(j); // 获取当前字符
		if (ch == '.') {
			if (dotAdded) {
				break; // 如果已经有小数点，则遇到第二个小数点时停止
			} else {
				dotAdded = true; // 标记小数点已添加
			}
		} else if (!isdigit(ch)) {
			break; // 非数字字符停止
		}
		num.push_back(ch); // 将字符添加到结果字符串
	}
	nextPos = j; // 设置下一个位置
	return atof(num.c_str()); // 将字符串转换为浮点数并返回
};

int Utils::extractInt(const std::string& line, size_t pos){
	int len = line.length(); // 获取字符串长度
	int i = pos; // 从指定位置开始
	for (; i < len; ++i) {
		if (isdigit(line.at(i))) break; // 找到数字，退出循环
	}
	if (i == len) throw Error("Current line does not contain integers."); // 如果到达行尾仍未找到数字，抛出错误
	std::string num(1, line.at(i)); // 将第一个数字添加到结果字符串
	char ch; // 存储当前字符
	for (int j = i + 1; j < len; ++j) {
		ch = line.at(j); // 获取当前字符
		if (!isdigit(ch)) break; // 非数字字符停止
		num.push_back(ch); // 将字符添加到结果字符串
	}
	return atoi(num.c_str()); // 将字符串转换为整数并返回
};
		


std::string Utils::deleteWhiteSpaces(const std::string &line){
	std::string lineWithoutSpaces(line); // 复制原始字符串
	lineWithoutSpaces.erase(std::remove_if(lineWithoutSpaces.begin(), lineWithoutSpaces.end(), isspace), 
		lineWithoutSpaces.end()); // 使用remove_if和isspace删除所有空白字符
	return lineWithoutSpaces; // 返回处理后的字符串
}

std::string Utils::getSubString(const std::string& first, const std::string& last, const std::string& line){
	size_t found = line.find(first); // 查找第一个分隔符
	if (found == std::string::npos) {
		std::string message = first; 
		message.append(" is missing in line: ");
		message.append(line); // 构建错误消息
		Error er(message); // 创建错误对象
		throw er; // 抛出错误
	}
	size_t foundEnd = line.find(last); // 查找第二个分隔符
	if (foundEnd == std::string::npos) {
		std::string message = last; ;
		message.append(" is missing in line: ");
		message.append(line); // 构建错误消息
		Error er(message); // 创建错误对象
		throw er; // 抛出错误
	}
	if (found > foundEnd) {
		std::string message = "Field name or value is missing in line: ";
		message.append(line); // 构建错误消息
		Error er(message); // 创建错误对象
		throw er; // 抛出错误
	}
	if (foundEnd - found <= 1) return ""; // 如果两个分隔符相邻，返回空字符串
	return line.substr(found + 1, foundEnd - found - 1); // 提取并返回两个分隔符之间的子字符串
};

std::string Utils::skipOneLineComment(const std::string& comment, const std::string& line){
	size_t found = line.find(comment); // 查找注释标记
	if (found != std::string::npos) {
		if (found == 0) {
			return ""; // 如果整行都是注释，返回空字符串
		} else {
			return line.substr(0, found - 1); // 返回注释前的内容
		}
	}
	return line; // 如果没有注释，返回原始行
};

void Utils::extractName(const char* filepath, std::string& name){
	
	// 文件路径格式: "/bla/bla/bla/problemName.someextension"
	// 我们尝试找出包含"problemName"的区间[begin; end)
	const char* end = filepath + strlen(filepath); // 指向文件路径末尾

	// 我们回退"end"
	// 找到最后一个点的位置（如果没有点，则为filepath）
	while(end > filepath) {
		if(*(end-1) == '.'){
			--end; // 将end指向点的位置
			break;
		}
		--end; // 向前移动end
	}

	const char* begin = end; // 初始化begin为end

	while(      begin  > filepath
        && *(begin-1) != '/'
		&& *(begin-1) != '\\'){
		--begin; // 向前移动begin直到找到路径分隔符
	}

	name.assign(begin, end); // 将[begin, end)之间的字符赋值给name
};

bool Utils::isDigits(const std::string &str){
	bool hasPoint = false; // 标记是否有小数点
	int length = str.length(); // 获取字符串长度
	for (int i = 0; i < length; ++i) {
		if (!isdigit(str.at(i))) {
			if (str.at(i) == '.' && !hasPoint && i != length-1) {
				hasPoint = true; // 如果是首次出现的小数点且不在最后位置，标记为已有小数点
			} else {
				return false; // 其他情况返回false
			}
		}	
	}
	return true; // 所有字符都是数字或合法小数点，返回true
   
};

bool Utils::isInt(const std::string &str){
	return str.find_first_not_of("0123456789") == std::string::npos; // 检查字符串是否只包含数字
};
