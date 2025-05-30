// #include <stdlib.h> // 注释掉的标准库引用
#include <iostream> // 包含输入输出流

#include "Params.h" // 包含参数类头文件
#include "Error.h" // 包含错误处理类头文件
#include "UtilsString.h" // 包含字符串工具类头文件

Params::Params(){ // 构造函数
	
};

Params::~Params(){ // 析构函数

};
 
std::string Params::getParam(const std::string &name){ // 获取参数值方法
	std::map<std::string, std::string>::iterator it = paramDict_.find(name); // 在参数字典中查找指定名称
	if (it == paramDict_.end()) { // 如果未找到参数
		throw Error("Unexpected parameter name: " + name); // 抛出错误
	}
	return it->second; // 返回参数值
};

std::string Params::getParamWoSpaces(const std::string &name){ // 获取无空格参数值方法
	return Utils::deleteWhiteSpaces(getParam(name)); // 删除参数值中的空格并返回
};

void Params::addParameter(const std::string &field, const std::string &value){ // 添加参数方法
	if ((paramDict_.insert(std::pair<std::string, std::string>(field, value))).second == false ) { // 如果插入失败（已存在该参数）
		std::string message = "Field: "; // 创建错误消息
		message.append(field); // 添加字段名
		message.append(" was already added with value: "); // 添加提示文本
		message.append(value); // 添加值
		Error er(message); // 创建错误对象
		throw er; // 抛出错误
	}
};

void Params::print(){ // 打印所有参数方法
	std::map<std::string, std::string>::iterator it = paramDict_.begin(); // 获取参数字典开始迭代器
	for(; it != paramDict_.end(); ++it) { // 遍历所有参数
		std::cout << "Field: " << it->first << " Value: " << it->second << std::endl; // 输出参数名和值
	}
};

std::string Params::getAutoFileName(char del){ // 获取自动生成文件名方法
	std::string netName; // 网络名称
	Utils::extractName(getParam("NETWORK").c_str(), netName); // 从NETWORK参数中提取名称
	std::string attributes[] = {"ALGORITHM", "EQUILIBRATION", "APPROACH", "LINE_SEARCH", 
			"NEWTON_STEPS", "ShPathAlgo", "BIOBJ_SHPATH_P2P", "BLS_BOUNDS", "USE_EXISTING_PATHS", 
			"ART2_TOLLS", "INDIFF_CURVE_TYPE"}; // 定义属性数组
	int nbAtt = sizeof(attributes) / sizeof(std::string); // 计算属性数量
	std::string attributeValue(""); // 初始化属性值
	std::string retVal(netName + del); // 初始化返回值，以网络名称开始
	for (int i = 0; i < nbAtt; ++i) { // 遍历所有属性
		try {
			attributeValue = getParamWoSpaces(attributes[i]); // 尝试获取属性值
		} catch (Error e){
			attributeValue = ""; // 捕获异常时设置属性值为空
		}
		if (attributeValue == "yes") attributeValue = "withPaths"; // 如果属性值为"yes"，则替换为"withPaths"
		if (attributeValue != "") retVal.append(attributeValue + del); // 如果属性值非空，则添加到返回值
	}
	if (!getParamWoSpaces("USE_PATH_COST_DOMINATION").empty()) retVal.append("pathCostDomination"); // 处理路径成本支配参数
	if (!getParamWoSpaces("UseP2PShPathWithRandomReturn").empty()) retVal.append("_rndReturn"); // 处理随机返回参数
	std::string prob = getParamWoSpaces("FIXED_PROBABILITY"); // 获取固定概率参数
	if (!prob.empty()) retVal.append("_prob_" + prob); // 处理固定概率参数
	return retVal; // 返回生成的文件名
};
