// 包含必要的头文件
#include "LatexNetOutputLinksExploredInBSP.h"
#include "BiObjLabelContainer.h"
#include "StarNetwork.h"
#include "StarLink.h"

#include <sstream>

// 构造函数1：初始化网络、已访问链接和标签容器
LatexNetOutputLinksExploredInBSP::LatexNetOutputLinksExploredInBSP(StarNetwork* net, 
							const std::vector<StarLink*>& seenLinks,
							const BiObjLabelContainer& labels) : 
							LatexNetOutput(net), seenLinks_(seenLinks),
							labels_(labels) {

};

// 构造函数2：额外包含缺失ID的日志文件
LatexNetOutputLinksExploredInBSP::LatexNetOutputLinksExploredInBSP(StarNetwork* net, 
							const std::vector<StarLink*>& seenLinks,
							const BiObjLabelContainer& labels,
			 				const std::string& logFileForMissingIDs) :
			 				LatexNetOutput(net, logFileForMissingIDs),
			 				seenLinks_(seenLinks), labels_(labels) {

};

// 析构函数
LatexNetOutputLinksExploredInBSP::~LatexNetOutputLinksExploredInBSP() {

};

// 获取线条标签，用于在LaTeX中显示
std::string LatexNetOutputLinksExploredInBSP::getLineLabel(StarLink* link, 
				const std::string& aboveOrBelow) {
	return "-- node[sloped," + aboveOrBelow + "]{}";
};

// 生成线条选项，根据链接是否被访问过返回不同的颜色
std::string LatexNetOutputLinksExploredInBSP::generateLineOptions(const FlowInfo& flowInfo, 
		StarLink* link) {
	if (seenLinks_[link->getIndex()] != NULL) {
		return "->, red";  // 已访问的链接显示为红色
	}
	return "->, blue";     // 未访问的链接显示为蓝色
};

// 判断是否绘制该链接
bool LatexNetOutputLinksExploredInBSP::plotLink(StarLink* link) const {
	return true;  // 默认绘制所有链接
};

// 获取节点标签，包含节点ID和标签数量
std::string LatexNetOutputLinksExploredInBSP::getNodeLabel(int nodeID) const {
	std::stringstream ss;
	ss << nodeID << ", " << labels_.getNbLabels(net_->getNodeIndex(nodeID));
	return ss.str();
};