#ifndef PARAMS_
#define PARAMS_

#include <string> // 包含字符串库
#include <map> // 包含映射容器库

/** \brief 此类用于存储从*.params或文本文件加载的参数。
	\details *.params文件的格式如下：\<属性名\> : {属性值}。
	允许使用以下格式的注释 \b/\b/ 或 \b/\b* \b*\b/。有关现有属性的描述，
	请参见文件 <a href="../../input.params">input.params</a> 和
	<a href="index.html">主页</a>。
*/ 
class Params { // 参数类
	public:
		Params(); // 构造函数
		~Params(); // 析构函数
		
		/** @return 以字符串格式返回给定属性 \b name 的值。
			如果给定属性不存在，则抛出错误。
		*/
		std::string getParam(const std::string &name); // 获取参数值方法
		
		/** 与getParam()相同，但返回的字符串不带任何空格。
		*/
		std::string getParamWoSpaces(const std::string &name); // 获取无空格参数值方法

		/** 在内部数据结构中保存给定属性及其值。
			所有属性都被假定为唯一的。如果尝试添加一个已存在的属性，
			则抛出错误。
		*/
		void addParameter(const std::string &field, const std::string &value); // 添加参数方法

		/** 在屏幕上打印所有属性及其对应值。
		*/
		void print(); // 打印参数方法
		
		/** 使用以下格式为文件生成自动名称：
			网络名称_算法_平衡_方法_线搜索_NEWTON_STEPS_ShPathAlgo
			+ 双目标最短路径参数（如果有）。
			默认分隔符是_。
			如果未提供某个属性，将跳过它们。
		*/
		std::string getAutoFileName(char del = '_'); // 获取自动生成文件名方法
		
	private:
		std::map<std::string, std::string> paramDict_; // 参数字典，存储属性名和值
};

#endif
