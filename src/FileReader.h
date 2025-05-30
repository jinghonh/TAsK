#ifndef FILE_READER  // 防止头文件重复包含
#define FILE_READER

#include <string>    // 包含字符串类
#include <iostream>  // 包含输入输出流
#include <fstream>   // 包含文件流

/** \brief 这个类简化了与文件读取相关的操作接口
    (类似于外观模式)。
*/
class FileReader {
	public:
		/** @param fileName 现有文件的路径。如果文件无法打开则抛出错误。
		*/
		FileReader(const std::string& fileName);  // 构造函数
		~FileReader();  // 析构函数
		
		/** 此方法检查是否仍可以从文件中读取信息。
			@return 如果到达文件末尾则返回false，否则返回true。
		*/
		bool isGood() const;  // 检查文件状态
		
		/** 此方法从文件中读取一行并返回。
		*/
		std::string getNextLine();  // 获取下一行内容
		
	private:
		std::ifstream file_;  // 文件输入流
		const std::string name_;  // 文件名
};

#endif  // 结束头文件保护
