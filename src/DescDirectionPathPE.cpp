#include "DescDirectionPathPE.h"  // 包含头文件

#include <cassert>  // 包含断言头文件

// 构造函数实现，初始化基类
DescDirectionPathPE::DescDirectionPathPE(FPType delta) : DescDirectionPath(delta){
 
};
 
// 析构函数实现
DescDirectionPathPE::~DescDirectionPathPE(){
  
};
  
// 创建路径方向的方法实现
// @param size 路径数量
// @param paths 路径列表
// @param isEquilibrated 是否达到平衡状态
// @return 返回路径方向数组
PathAndDirection** DescDirectionPathPE::createPathDirection(int &size, const std::list<Path*> &paths, 
                                              bool &isEquilibrated){
  	
  	// 寻找最短路径和最长路径
  	Path *minPath = findMinPath(paths);  // 获取最短路径
  	FPType minDist = minPath->getCurrCost();  // 获取最短路径成本
  	Path *maxPath = findMaxPath(paths, minPath);  // 获取最长路径
  	FPType maxDist = maxPath->getCurrCost();  // 获取最长路径成本
  	
  	// 计算方向
  	FPType dir = minDist - maxDist;  // 计算方向值
  	if (-dir <= delta_) {  // 如果方向值小于等于阈值
  		isEquilibrated = true;  // 设置平衡状态
  	  size = 0;  // 重置大小
		  return NULL;  // 返回空指针
  	}

    isEquilibrated = false;  // 设置非平衡状态
    size = 2;  // 设置大小为2
    PathAndDirection **returnSet = new PathAndDirection*[size];  // 创建返回数组
    for (int i = 0; i < size; ++i) {  // 初始化数组
      returnSet[i] = new PathAndDirection();  // 创建新对象
    }

  	(returnSet[0])->setPath(maxPath);  // 设置最长路径
  	(returnSet[0])->setDirection(dir);  // 设置方向值
  	(returnSet[1])->setPath(minPath);  // 设置最短路径
  	(returnSet[1])->setDirection(-dir);  // 设置反向方向值
  	return returnSet;  // 返回结果数组
};
