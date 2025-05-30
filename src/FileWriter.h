#ifndef FILE_WRITER  // 防止头文件重复包含
#define FILE_WRITER

#include <fstream>   // 包含文件流操作的头文件
#include <string>    // 包含字符串操作的头文件

/** \brief 这个类简化了与文件写入操作相关的接口（类似于外观模式）。
*/
class FileWriter {
	public:
		/** @param fileName 文件路径。如果文件不存在将会被创建。
			@param append （默认值为false）必须指定文件是追加（true）还是覆盖（false）。
		*/
		explicit FileWriter(const std::string& fileName, bool append = false);  // 显式构造函数
		~FileWriter();  // 析构函数
		
		/** 向文件写入一行。
			\note 不会自动添加行尾符号。
		*/
		void writeLine(const std::string& line);  // 写入一行内容
		/** 向文件添加一个空行。
		*/
		void addEmptyLine();  // 添加空行
		
		/** @return 返回文件名。
		*/
		std::string getName() const;  // 获取文件名
		
	private:
		const std::string name_;  // 存储文件名
		std::ofstream file_;      // 文件输出流对象
};

#endif  // 结束头文件保护
