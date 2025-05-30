#ifndef UTILS_STRING
#define UTILS_STRING

#include "UsedTypes.h" // 包含自定义类型定义

#include <string> // 包含标准库字符串

/** \brief 定义用于字符串操作的有用函数。
*/
namespace Utils {
	
	/** 如果值为零则抛出错误。
		*/
	void checkEmptyValue(int value); // 检查值是否为空（零）
	/** 给定行，从位置pos开始提取整数值。
		如果行不包含数字则抛出错误。
		@return 提取的整数值。
	*/
	int extractInt(const std::string& line, size_t pos); // 从字符串中提取整数
	/** 给定行，从位置pos开始提取浮点值。
		将提取的浮点值的结束位置写入nextPos。
		如果行不包含数字则抛出错误。
		@return 提取的浮点值。
	*/
	FPType extractNumber(const std::string& line, size_t pos, size_t &nextPos); // 从字符串中提取浮点数

	/** @return 字符串 \b line 中第一个在 \b first 和 \b last 字符串之间的子字符串。
		例如
		@code getSubString("<",">","{example}:<value>");
		@endcode
		将返回 \b value。
		\note \b first 和 \b last 假定为不同的字符串，否则方法将返回空字符串。
	*/
	std::string getSubString(const std::string& first, const std::string& last, 
		const std::string& line); // 获取指定分隔符之间的子字符串

	/** 文件路径 \b filePath 的格式："/bla/bla/bla/problemName.someextension"
		@return \b problemName
		@author Sylvain Rosembly
	*/
	void extractName(const char* filepath, std::string& name); // 从文件路径中提取文件名

	/** 从字符串中提取单行注释。例如：
		@code skipOneLineComment("//", "i = 10*j + 4; //some comments "); @endcode
		将返回 i = 10*j + 4;
	*/
	std::string skipOneLineComment(const std::string& comment, const std::string& line); // 跳过单行注释

	/** 获取字符串 \b line 并返回一个不包含 \b line 中的空白字符的新字符串。
	*/
	std::string deleteWhiteSpaces(const std::string &line); // 删除字符串中的空白字符

	/** @return 如果 \b str 是一个数字（可能是非整数），则返回true，否则返回false。
		\note 只有这种格式 *.* （或 *）的字符串，其中 * 是整数，被识别为数字
		（1E-10不被视为数字）。
	*/
	bool isDigits(const std::string &str); // 检查字符串是否为数字
	
	/** @return 如果 \b str 是整数，则返回true，否则返回false。
	*/
	bool isInt(const std::string &str); // 检查字符串是否为整数

}

#endif // UTILS_STRING头文件结束
