#include "PAS.h" // 包含PAS（备选路段对）头文件
#include "DAGraphTapas.h" // 包含TAPAS有向无环图头文件
#include "Utils.h" // 包含工具函数头文件
#include "StarLink.h" // 包含星型链接头文件

#include <cassert> // 包含断言头文件
#include <limits> // 包含限制头文件，用于数值极限
#include <iostream> // 包含输入输出流头文件

FPType PAS::zeroFlow_ = 0.0; // 初始化静态零流量阈值
FPType PAS::dirTol_ = 0.0; // 初始化静态方向容差阈值

PAS::PAS(FPType zeroFlow, FPType dirTol) : cheapCost_(0.0), expCost_(0.0), totalShift_(0.0),
 							cheapSegm_(0), relevantOrigins_(), nbFlowMoves_(0) {  // 构造函数，初始化所有成员变量
	zeroFlow_ = zeroFlow; // 设置静态零流量阈值
	dirTol_ = dirTol; // 设置静态方向容差阈值
}; 

PAS::~PAS() { // 析构函数

}; 

bool PAS::isUnused() { // 判断PAS是否未使用的方法
	int retVal = nbFlowMoves_ ; // 获取流量移动计数器值
	nbFlowMoves_ = 0; // 重置流量移动计数器
	return retVal < 0; // 如果计数器小于0则PAS未使用
}; 

void PAS::pushFrontToCheap(StarLink *link) { // 将链接添加到廉价路段前端的方法
	segments_[cheapSegm_].push_front(link); // 在廉价路段前端添加链接
	cheapCost_ += link->getTime(); // 增加廉价路段成本
}; 

void PAS::pushFrontToExp(StarLink *link) { // 将链接添加到昂贵路段前端的方法
	segments_[1 - cheapSegm_].push_front(link); // 在昂贵路段前端添加链接
	expCost_ += link->getTime(); // 增加昂贵路段成本
}; 

void PAS::pushBackToCheap(StarLink *link){ // 将链接添加到廉价路段后端的方法
	segments_[cheapSegm_].push_back(link); // 在廉价路段后端添加链接
	cheapCost_ += link->getTime(); // 增加廉价路段成本
};

void PAS::pushBackToExp(StarLink *link){ // 将链接添加到昂贵路段后端的方法
	segments_[1 - cheapSegm_].push_back(link); // 在昂贵路段后端添加链接
	expCost_ += link->getTime(); // 增加昂贵路段成本
};		
		
StarLink* PAS::getLastCheapLink(){ // 获取廉价路段最后一个链接的方法
	if (segments_[cheapSegm_].empty()) return NULL; // 如果廉价路段为空，返回NULL
	return segments_[cheapSegm_].back(); // 返回廉价路段的最后一个链接
};

StarLink* PAS::getLastExpLink(){ // 获取昂贵路段最后一个链接的方法
	if (segments_[1 - cheapSegm_].empty()) return NULL; // 如果昂贵路段为空，返回NULL
	return segments_[1 - cheapSegm_].back(); // 返回昂贵路段的最后一个链接
};
				
bool PAS::addOrigin(DAGraphTapas* dag) { // 添加起点菊花的方法
	assert(dag != NULL); // 断言参数不为空
	return relevantOrigins_.insert(dag).second; // 将菊花插入相关起点集合，并返回是否插入成功
}; 

bool PAS::checkIfEffective(FPType cost, FPType v, int index, DAGraphTapas* dag){ // 检查PAS是否有效的方法
	return checkIfCostEffective(cost) && checkIfFlowEffective(v, index, dag); // 同时检查成本和流量有效性
};

bool PAS::checkIfFlowEffective(FPType v, int index, DAGraphTapas* dag) const{ // 检查流量是否有效的方法
	assert(index >= 0); // 断言索引非负
	FPType minFlow = std::numeric_limits<FPType>::infinity(); // 初始化最小流量为无穷大
	FPType flow = 0.0; // 初始化流量为0
	int expIndex = 1 - cheapSegm_; // 计算昂贵路段索引
	for (std::list<StarLink*>::const_iterator it = segments_[expIndex].begin(); 
						it != segments_[expIndex].end(); ++it) { // 遍历昂贵路段的所有链接
		flow = dag->getOriginFlow((*it)->getIndex()); // 获取链接的起点流量
		if (flow < minFlow) minFlow = flow; // 更新最小流量
	}
	return (minFlow >= v * dag->getOriginFlow(index)); // 返回最小流量是否大于等于指定阈值
};

bool PAS::checkIfCostEffective(FPType cost) const{ // 检查成本是否有效的方法
	return (expCost_ - cheapCost_ >= cost); // 返回成本差异是否大于等于指定阈值
};

bool PAS::moveFlow() { // 移动流量的方法
	
	--nbFlowMoves_; // 减少流量移动计数器
	bool tmp = true; // 初始化临时变量为true
	if (recalcPASCosts() >= dirTol_) { // 如果重新计算的成本差异大于等于方向容差
		
		FPType dFlow = calculateFlowShift(); // 计算适当的流量移动量和每个起点的最小移动量
											 
		
		StarLink* link = NULL; // 初始化链接指针为空
		int expIndex = 1 - cheapSegm_; // 计算昂贵路段索引
		// 更新起点流量
		DAGraphTapas* dagPointer = NULL; // 初始化菊花指针为空
		for (SetType::iterator dag = relevantOrigins_.begin(); dag != relevantOrigins_.end(); ++dag) { // 遍历所有相关起点
			if (totalShift_ > 0.0) { // 如果总移动量大于0
				dagPointer = *dag; // 获取当前菊花指针
				FPType flowShift = dagPointer->getMinShift() / totalShift_ * dFlow; // 计算该菊花的流量移动量
				if (flowShift > zeroFlow_) { // 如果流量移动量大于零流量阈值
					tmp = false; // 设置临时变量为false，表示需要移动流量
					for (std::list<StarLink*>::iterator it = segments_[cheapSegm_].begin(); 
								it != segments_[cheapSegm_].end(); ++it) { // 遍历廉价路段的所有链接
						dagPointer->addOriginFlowAndCreateLink(*it, flowShift); // 向廉价路段链接添加流量
																				// 如果链接不在菊花中，
																				// 则将其添加到菊花
						
					}
					
					for (std::list<StarLink*>::iterator it = segments_[expIndex].begin(); 
									it != segments_[expIndex].end(); ++it) { // 遍历昂贵路段的所有链接
						link = *it; // 获取当前链接
						int index = link->getIndex(); // 获取链接索引
						if (dagPointer->getOriginFlow(index) - flowShift < zeroFlow_) { // 如果移除流量后小于零流量阈值
							dagPointer->setOriginFlowToZero(index); // 将流量设为零
						} else { // 否则
							dagPointer->addOriginFlowAndCreateLink(link, -flowShift); // 从昂贵路段链接减少流量
																					  // 如果链接不在菊花中，
																					  // 则将其添加到菊花
						}
						
					}
				}
			} else { // 如果总移动量为0
				assert(dFlow == 0.0); // 断言流量移动量为0
			}
		}
		
		// 更新链接流量和成本
		if (tmp == false) { // 如果需要移动流量
			for (std::list<StarLink*>::iterator it = segments_[cheapSegm_].begin(); it != segments_[cheapSegm_].end(); 
														++it) { // 遍历廉价路段的所有链接
				link = *it; // 获取当前链接
				link->addFlow(dFlow); // 添加流量
				link->updateTime(); // 更新时间（成本）
			}
			for (std::list<StarLink*>::iterator it = segments_[expIndex].begin(); it != segments_[expIndex].end(); 
														++it) { // 遍历昂贵路段的所有链接
				link = *it; // 获取当前链接
				if (link->getFlow() - dFlow < zeroFlow_) { // 如果移除流量后小于零流量阈值
					link->setFlowToZero(); // 将流量设为零
				} else { // 否则
					link->addFlow(-dFlow); // 减少流量
				}
				link->updateTime(); // 更新时间（成本）

			}
			++nbFlowMoves_; // 增加流量移动计数器
			return true; // 返回true，表示已移动流量
		} 
		
	}
	return false; // 返回false，表示未移动流量
}; 

//
FPType PAS::calculateFlowShift(){ // 计算流量移动量的方法
	// 计算最大可行移动量
	totalShift_ = 0.0; // 初始化总移动量为0
	DAGraphTapas* dagPointer = NULL; // 初始化菊花指针为空
	for (SetType::iterator dag = relevantOrigins_.begin(); dag != relevantOrigins_.end(); ++dag) { // 遍历所有相关起点
		FPType minFlowShift = std::numeric_limits<FPType>::infinity(); // 初始化最小流量移动量为无穷大
		dagPointer = *dag; // 获取当前菊花指针
		FPType oFlow = 0.0; // 初始化流量为0
		for (std::list<StarLink*>::iterator it = segments_[1 - cheapSegm_].begin(); 
												it != segments_[1 - cheapSegm_].end(); ++it) { // 遍历昂贵路段的所有链接
			
			oFlow = dagPointer->getOriginFlow((*it)->getIndex()); // 获取链接的起点流量
			if (oFlow < minFlowShift) minFlowShift = oFlow; // 更新最小流量移动量
		}
		dagPointer->setMinShift(minFlowShift); // 设置菊花的最小移动量
		totalShift_ += minFlowShift; // 增加总移动量
	}
	
	FPType dFlow = getFlowShift(); // 获取流量移动量
	assert(dFlow >= 0.0 && dFlow == dFlow); // 断言流量移动量非负且是有效数字
	if (dFlow > totalShift_) { // 如果流量移动量大于总移动量
		dFlow = totalShift_; // 将流量移动量设为总移动量
	}
	return dFlow; // 返回流量移动量
};

FPType PAS::getFlowShift(){ // 获取流量移动量的方法
	return (expCost_ - cheapCost_) / Utils::calculatePathDerivativeForDisjoint(segments_[cheapSegm_],
								segments_[1 - cheapSegm_]); // 返回成本差异除以路径导数
};

FPType PAS::calcSegCost(int index){ // 计算路段成本的方法
	FPType segCost = 0.0; // 初始化路段成本为0
	for (std::list<StarLink*>::iterator it = segments_[index].begin(); it != segments_[index].end(); ++it) { // 遍历路段的所有链接
		segCost += (*it)->getTime(); // 累加链接时间（成本）
	}
	return segCost; // 返回路段成本
};

FPType PAS::getCostDiff(){ // 获取成本差异的方法
	return expCost_ - cheapCost_; // 返回昂贵路段成本与廉价路段成本的差值
};

FPType PAS::recalcPASCosts(){ // 重新计算PAS成本的方法
	FPType cost0 = 	calcSegCost(0); // 计算路段0成本
	FPType cost1 = 	calcSegCost(1); // 计算路段1成本
	if (cost0 < cost1) { // 如果路段0成本小于路段1成本
		cheapSegm_ = 0; // 设置廉价路段索引为0
		cheapCost_ = cost0; // 更新廉价路段成本
		expCost_ = cost1; // 更新昂贵路段成本
	} else { // 否则
		cheapSegm_ = 1; // 设置廉价路段索引为1
		cheapCost_ = cost1; // 更新廉价路段成本
		expCost_ = cost0; // 更新昂贵路段成本
	}
	return expCost_ - cheapCost_; // 返回成本差异
};

void PAS::print()const  { // 打印PAS信息的方法
	StarLink *link = NULL; // 初始化链接指针为空
	std::cout << "cheap segment cost = " << cheapCost_ << " "; // 打印廉价路段成本
	for(std::list<StarLink*>::const_iterator it = segments_[cheapSegm_].begin(); it != segments_[cheapSegm_].end(); ++it){ // 遍历廉价路段的所有链接
		link = *it; // 获取当前链接
		std::cout << "link " << link->getIndex() << " [" << link->getNodeFromIndex() << " ," << 
			link->getNodeToIndex() << "] time = " << link->getTime() << " flow = " << link->getFlow() << " "; // 打印链接信息
	}
	std::cout << std::endl; // 换行
	std::cout << "expensive segment cost = " << expCost_ << " "; // 打印昂贵路段成本
	for(std::list<StarLink*>::const_iterator it = segments_[1 - cheapSegm_].begin(); 
										it != segments_[1 - cheapSegm_].end(); ++it){ // 遍历昂贵路段的所有链接
		link = *it; // 获取当前链接
		std::cout << "link " << link->getIndex() << " [" << link->getNodeFromIndex() << " ," << 
			link->getNodeToIndex() << "] time = " << link->getTime() << " flow = " << link->getFlow() << " "; // 打印链接信息
	}
	std::cout << std::endl; // 换行
	std::cout << "Origins: "; // 打印起点标题
	for(SetType::const_iterator it = relevantOrigins_.begin(); it != relevantOrigins_.end(); ++it){ // 遍历所有相关起点
		std::cout << (*it)->getOriginIndex() << " "; // 打印起点索引
	}
	std::cout << std::endl; // 换行
}; 

int PAS::getNbLinks() const{ // 获取链接总数的方法
	return segments_[0].size() + segments_[1].size(); // 返回两个路段的链接数之和
};

int PAS::getNbOrigins() const{ // 获取起点总数的方法
	return relevantOrigins_.size(); // 返回相关起点集合的大小
};

