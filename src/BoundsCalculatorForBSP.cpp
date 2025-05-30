// 包含必要的头文件
#include "BoundsCalculatorForBSP.h"
#include "ReverseNetworkWithTolls.h"
#include "StarNetwork.h"
#include "StarNode.h"

#include <cassert>

// 构造函数实现
BoundsCalculatorForBSP::BoundsCalculatorForBSP(StarNetwork& net, const TollContainerType& tolls): 
			zeroFlowTimes_(net.getNbNodes() * net.getNbNodes(), 0),  // 初始化零流量时间数组
			tolls_(net.getNbNodes() * net.getNbNodes(), 0),          // 初始化收费数组
			originalNet_(&net), linkTolls_(tolls) {                  // 保存原始网络和收费信息
};

// 初始化边界值
void BoundsCalculatorForBSP::initializeBounds() {
	reverseStar_ = getNetwork(*originalNet_);                        // 获取反向网络
	spp_ = new LabelCorrectingAlWithoutZonesCheck(reverseStar_);    // 创建标签修正算法对象

	StarNetwork* netTolls = getNetworkWithTolls(*originalNet_, linkTolls_);  // 获取带收费的反向网络
	LabelCorrectingAlWithoutZonesCheck sppWithTolls(netTolls);      // 创建带收费的标签修正算法对象

    StarNetwork* oppositeNet = getOppositeNet();                    // 获取对向网络
    int nbNodes = oppositeNet->getNbNodes();                        // 获取节点数量

    StarNode* node = NULL;
	for(int i = 0; i < nbNodes; ++i){                               // 遍历所有节点
    	node = reverseStar_->getNodeWithLinks(i);
    	if (node != NULL) {
	        spp_->calculate(i);                                     // 计算最短路径
	        sppWithTolls.calculate(i);                              // 计算带收费的最短路径
	        
	        StarNode* tollNode = netTolls->getNodeWithLinks(i);     // 获取带收费的节点
	        assert(tollNode != NULL);                               // 确保节点存在
	        assert(node->getID() == tollNode->getID());             // 确保节点ID匹配

	        int iForwardIndex = oppositeNet->getNodeIndex(node->getID());  // 获取正向索引

	        for (int j = 0; j < nbNodes; ++j){                      // 遍历所有目标节点
	            
	            int jID_rNet = reverseStar_->findID(j);             // 获取反向网络中的节点ID
	            int jID_tollNet = netTolls->findID(j);              // 获取带收费网络中的节点ID
	           
	            assert(jID_tollNet == jID_rNet);                    // 确保节点ID一致
	            if (jID_rNet != -1) {                               // 如果节点存在

		            int jForwardIndex = oppositeNet->getNodeIndex(jID_rNet);  // 获取目标节点的正向索引

		            int lineIndex = iForwardIndex*nbNodes + jForwardIndex;    // 计算数组索引
		            zeroFlowTimes_[lineIndex] = spp_->getCost(j);            // 存储零流量时间
		            if (zeroFlowTimes_[lineIndex] == std::numeric_limits<FPType>::infinity()) {
		            	tolls_[lineIndex] = std::numeric_limits<TollType>::infinity();  // 如果时间无穷大，收费也设为无穷大
		            } else {
			            tolls_[lineIndex] = sppWithTolls.getCost(j);         // 存储收费值
			        }
		            if (tolls_[lineIndex] < 0) {                              // 如果收费为负
		             	tolls_[lineIndex] = std::numeric_limits<TollType>::infinity();  // 设为无穷大
		            }
		            assert(tolls_[lineIndex] >= 0);                           // 确保收费非负
		         } 
	        }
	    }
    }

    delete netTolls;                                                // 释放带收费网络的内存
};

// 获取对向网络
StarNetwork* BoundsCalculatorForBSP::getOppositeNet() {
	return originalNet_;
};

// 获取反向网络
StarNetwork* BoundsCalculatorForBSP::getNetwork(StarNetwork& net) {
	ReverseNetwork rNetCreator;
	std::cout << "创建用于常规边界的反向网络" << std::endl;
	return rNetCreator.createReverseStar(net);
};

// 获取带收费的反向网络
StarNetwork* BoundsCalculatorForBSP::getNetworkWithTolls(StarNetwork& net, const TollContainerType& tolls) {
	ReverseNetworkWithTolls rNetCreatorTolls(tolls);
	std::cout << "创建用于常规边界的带收费反向网络" << std::endl;
	return rNetCreatorTolls.createReverseStar(net);
};

// 析构函数
BoundsCalculatorForBSP::~BoundsCalculatorForBSP() {
	if (reverseStar_ != originalNet_) delete reverseStar_;          // 如果反向网络不是原始网络，则删除
	delete spp_;                                                    // 删除标签修正算法对象
};

// 更新时间边界（待实现）
void BoundsCalculatorForBSP::updateTimeBounds(int destNodeIndex, int odPairIndex) {

};

// 获取收费下界
TollType BoundsCalculatorForBSP::getTollLowerBound(int nodeIndex, int destNodeIndex) {
	return tolls_[destNodeIndex*reverseStar_->getNbNodes() + nodeIndex];
};

// 获取时间下界
FPType BoundsCalculatorForBSP::getTimeLowerBound(int nodeIndex, int destNodeIndex) {
	return zeroFlowTimes_[destNodeIndex*reverseStar_->getNbNodes() + nodeIndex];
};