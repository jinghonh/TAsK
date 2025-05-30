#ifndef PATH_SET_PARSER
#define PATH_SET_PARSER 

#include <string>             // 包含字符串库

class PathSet;                // 前向声明路径集合类
class StarNetwork;            // 前向声明星型网络类

namespace PathSetParser {
	/** 从文本文件解析路径集合并将它们添加到paths中。
		@param paths 将添加所有路径集合的PathSet对象。
		@param net 网络。
		@param fileName 包含路径集合的文件路径。
	*/
 	void parse(PathSet& paths, const StarNetwork& net, const std::string& fileName);
};

#endif