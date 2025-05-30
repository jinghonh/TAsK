#ifndef UTILS_
#define UTILS_

#include "UsedTypes.h"

#include <list>
#include <string>

class Path;
class PathBasedFlowMove;
class StarNetwork;
class ODMatrix;
class SpiessFncCreator;
class PathSet;
class StarLink;

struct Coords {
	int nodeID;
	int x;
	int y;
} ;

/** \brief 这个命名空间定义了各种有用的函数。
*/
namespace Utils {

	/** 查找路径 \b path1 和 \b path2 的所有不相交链接，并将它们写入
		链表 \b list。
	*/
	void getDisjointLinks(Path *path1, Path *path2, std::list<StarLink*> &list, int nbLinks);

	/** 查找在 \b paths 中指定的路径的所有共同链接，并将它们写入
		链表 \b list。
	*/
	void getCommonLinks(PathBasedFlowMove *paths, std::list<StarLink*> &list, int nbLinks);

	/** 检查网络流约束是否在图的每个节点上都得到满足。
		@return 最大链接流偏差。
	*/
	FPType checkFeasibility(StarNetwork *net, ODMatrix *mat);

	/** 解析字符串 \b line 并将Spiess链接成本函数参数返回到
		SpiessFncCreator \b spCreator。
		@return 生成的Spiess函数的ID。
	*/
	int parseSpiessFnc(const std::string& input, SpiessFncCreator &spCreator, bool constFnc = false);
	
	/** 计算关于两条路径 \b path 和 \b minPath 之间流量转移的导数。
	*/
	FPType calculatePathDerivative(Path *path, Path *minPath, int nbLinks);
	
	/** 与calculatePathDerivative()相同，但不相交链接已知并存储在
		链表 \b list 中。
	*/
	FPType calculatePathDerivativeForDisjoint(const std::list<StarLink*> &list);
	
	/** 与calculatePathDerivative()相同，但不相交链接已知并存储在
		两个链表 \b list1 和 \b list2 中。
	*/
	FPType calculatePathDerivativeForDisjoint(const std::list<StarLink*> &list1,
											  const std::list<StarLink*> &list2);

	/** 从文件art3NodesFile读取坐标并将它们写入链表newCoords。
	*/
	void readART3Coordinates(const std::string& art3NodesFile, std::list<Coords>& newCoords);

	/** 将坐标从coords复制到newCoords，用于在net中存在的节点。
	*/
	void mapCoordsFromART3(const std::list<Coords>& coords, StarNetwork* net,
						   std::list<Coords>& newCoords);
	/** 将坐标写入文件fileName。
	*/
	void writeCoordsToFile(const std::list<Coords>& coords, const std::string& fileName);
	/** 非常简单的字符串分词实现。它依赖于stringstream分词字符串的能力。
	*/
	void tokenizeIntoStr(const std::string& lineToTokenize, std::string* result, int nbTokens);
	/** @return 如果nodeID存在于coords中，则返回具有填充属性的Coords对象，
			如果不存在，则所有属性设置为-1。
	*/
	Coords findCoords(const std::list<Coords>& coords, int nodeID);

	/** @return 在区间[0, maxVal]中的随机浮点数。
	*/
	FPType generateRndNumber(FPType maxVal);

}

#endif
