#ifndef LATEX_NET_OUTPUT_LINKS_SEEN_IN_BSP  // 防止头文件重复包含的宏定义
#define LATEX_NET_OUTPUT_LINKS_SEEN_IN_BSP 

#include "LatexNetOutput.h"  // 包含基类头文件

class BiObjLabelContainer;  // 前向声明双目标标签容器类

/** \brief 实现创建latex文件来显示双目标最短路径算法探索的链接

    所有被探索的链接用红色绘制，其他链接用蓝色显示。
    在每个节点旁边显示非支配标签的数量。
*/
class LatexNetOutputLinksExploredInBSP : public LatexNetOutput {  // 继承自LatexNetOutput的类定义
	public:
		// 构造函数1：初始化网络、已探索链接和标签容器
		LatexNetOutputLinksExploredInBSP(StarNetwork* net, 
							const std::vector<StarLink*>& seenLinks,
							const BiObjLabelContainer& labels);
		// 构造函数2：额外包含缺失ID的日志文件
		LatexNetOutputLinksExploredInBSP(StarNetwork* net, 
							const std::vector<StarLink*>& seenLinks,
							const BiObjLabelContainer& labels,
			 				const std::string& logFileForMissingIDs);
		~LatexNetOutputLinksExploredInBSP();  // 析构函数

	protected:
		// 获取链接标签的虚函数
		virtual std::string getLineLabel(StarLink* link, const std::string& aboveOrBelow);
		// 生成链接选项的虚函数
		virtual std::string generateLineOptions(const FlowInfo& flowInfo, StarLink* link);
		// 判断是否绘制链接的虚函数
		virtual bool plotLink(StarLink* link) const;
		// 获取节点标签的虚函数
		virtual std::string getNodeLabel(int nodeID) const;

	private:
		const std::vector<StarLink*>& seenLinks_;  // 已探索链接的引用
		const BiObjLabelContainer& labels_;  // 标签容器的引用

};
#endif