#ifndef TOLLS_MANAGEMENT // 防止头文件重复包含
#define TOLLS_MANAGEMENT 

#include "UsedTypes.h" // 包含自定义类型定义

#include <string> // 包含字符串库

class ShortestPath; // 前向声明最短路径类
class ODMatrix; // 前向声明OD矩阵类
class StarNetwork; // 前向声明星形网络类

/** \brief 定义用于操作收费的实用函数。
*/
namespace TollsManagement { // 收费管理命名空间

	/** 在区间[1, maxToll]内以概率 \b probability 生成随机收费，
		并将所有收费写入容器tolls。
	*/
	void generateRandomTolls(TollContainerType& tolls, FPType probabylity, TollType maxToll); // 生成随机收费方法
	/** 在最短路径上的区间[1, maxToll]内以概率 \b probability 生成随机收费，
		并将所有收费写入容器tolls。
	*/
	void generateRandomTollsOnShortestPath(TollContainerType& tolls, 
		FPType probabylity, TollType maxToll, ODMatrix* mat, ShortestPath *shPath); // 在最短路径上生成随机收费方法
	/** 基于描述网络的现有文件existingFile，将收费写入新文件newFile。
	*/
	void writeTollsToFile(const TollContainerType& tolls, const std::string& existingFile, 
		const std::string& newFile, StarNetwork* net); // 将收费写入文件方法
};

#endif