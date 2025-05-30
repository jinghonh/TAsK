#ifndef DEFAULT_NET_PARSER  // 防止头文件重复包含
#define DEFAULT_NET_PARSER 

#include "UsedTypes.h"  // 包含基本类型定义

#include <string>  // 包含字符串类

class StarNetwork;  // 前向声明StarNetwork类
class FileReader;   // 前向声明FileReader类

/** 内部工具结构体
*/
struct DataFromHeader { 
	int nbNodes;    // 节点数量
	int nbLinks;    // 链路数量
	int firstNode;  // 第一个节点
};

/** \brief 实现从文本文件解析网络的解析器
	
	文件格式描述见 <a href="http://www.bgu.ac.il/~bargera/tntp/"> 这里 </a>。
*/
class DefaultNetParser {
	public:
		/** @param fileWithNetwork 包含网络的文件路径
		*/
		DefaultNetParser(const std::string& fileWithNetwork);  // 构造函数
		virtual ~DefaultNetParser();  // 虚析构函数

		/** 从类构造函数指定的文件中解析网络并创建StarNetwork对象
			\note  假设所有链路都按照头节点的升序排序
		*/
		virtual StarNetwork* parseNetwork();  // 解析网络的方法

	protected:
		const std::string fileWithNetwork_;  // 存储网络文件路径

		/** 当网络文件也包含链路收费时的钩子方法，供派生类使用
		*/
		virtual void allocateTolls(int nbLinks) {};  // 分配收费数组
		/** 当网络文件也包含链路收费时的钩子方法，供派生类使用
		*/
		virtual void setToll(int linkIndex, TollType toll) {};  // 设置链路收费

	private:
		
		DataFromHeader parseDataFromHeader(FileReader& readFile);  // 解析文件头数据

};
#endif