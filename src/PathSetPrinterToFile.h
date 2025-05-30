#ifndef PATH_SET_PRINTER_TO_FILE // 防止头文件重复包含
#define PATH_SET_PRINTER_TO_FILE // 定义PATH_SET_PRINTER_TO_FILE宏

#include <string> // 包含字符串类

class ODMatrix; // 前向声明OD矩阵类
class PathSet; // 前向声明路径集合类
class ODSet; // 前向声明OD集合类
class Path; // 前向声明路径类

/** \brief 将路径集合打印到文本文件。
*/
class PathSetPrinterToFile { // 路径集合文件打印机类
	public:
		/** @param mat OD矩阵。
		*/
		PathSetPrinterToFile(ODMatrix* mat); // 构造函数
		~PathSetPrinterToFile(); // 析构函数

		/** 将路径集合写入文件。
			@param pathSet 指向PathSet对象的指针。
			@param fileName 路径集合将被写入的文件路径。
		*/
		void writePathSetToFile(PathSet* pathSet, const std::string& fileName) const; // 将路径集合写入文件的方法

	private:
		ODMatrix* mat_; // OD矩阵指针

		std::string createStringForOD(ODSet* od, long int index) const; // 为OD对创建字符串的私有方法
		std::string createStringForPath(Path* path, long int index) const; // 为路径创建字符串的私有方法
	
};
#endif // 结束头文件保护 