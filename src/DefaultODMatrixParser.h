#ifndef DEFAULT_OD_MATRIX_PARSER  // 防止头文件重复包含
#define DEFAULT_OD_MATRIX_PARSER 

#include "UsedTypes.h"  // 包含自定义类型定义

#include <string>  // 包含字符串处理库

class ODMatrix;  // 前向声明OD矩阵类
class StarNetwork;  // 前向声明星型网络类

/** \brief 实现从文本文件解析O-D矩阵的解析器。
    
    文件格式描述见 <a href="http://www.bgu.ac.il/~bargera/tntp/"> 这里 </a>。
*/
class DefaultODMatrixParser {
	public:
		/** @param fileWithODMatrix O-D矩阵文件的路径。
		*/
		DefaultODMatrixParser(const std::string& fileWithODMatrix);  // 构造函数
		~DefaultODMatrixParser();  // 析构函数

		/** 解析在类构造函数中指定的文件中的O-D矩阵
			并创建一个ODMatrix对象。
			\note 创建对象后会调用setIndexes()方法，
			因此无需再次调用。
		*/
		virtual ODMatrix* parseODMatrix(StarNetwork* net);  // 解析O-D矩阵的虚函数

	protected:
		const std::string fileWithODMatrix_;  // 存储O-D矩阵文件路径的成员变量

		/** 从posFrom位置开始从行中提取目的地，
			将其ID写入destID，需求量写入demand。
			@return 开始查找下一个目的地的行位置，
				如果未找到目的地则返回std::string::npos。
		*/
		virtual size_t extractDestination(size_t posFrom, const std::string &line, 
			int &destID, FPType &demand) const;  // 提取目的地信息的虚函数
     
};

#endif