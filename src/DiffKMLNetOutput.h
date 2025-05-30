#ifndef DIFF_KML_NET_OUTPUT  // 防止头文件重复包含
#define DIFF_KML_NET_OUTPUT

#include "KMLNetOutput.h"  // 包含基类头文件

/** \brief 该类实现了描述解决方案之间链路流量差异的KML文件
    (通常用于比较两个解决方案之间的差异)。
*/
class DiffKMLNetOutput : public KMLNetOutput {  // 继承自KMLNetOutput类
	public:
		/** @param net 网络
			@param flowDiff 包含链路流量差异的向量
			@param tolerance 描述哪些链路流量差异应被视为零
		*/
		DiffKMLNetOutput(StarNetwork* net, const std::vector<FPType>& flowDiff, FPType tolerance);  // 构造函数1
		DiffKMLNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs,  // 构造函数2
			const std::vector<FPType>& flowDiff, FPType tolerance);
		~DiffKMLNetOutput();  // 析构函数

	protected:
		virtual FPType calcLineWidth(StarLink* link);  // 计算线宽
		virtual std::string createColorFromLineWidth(FPType lineWidth);  // 根据线宽创建颜色
		virtual std::string createLineWidth(FPType lineWidth);  // 创建线宽
		virtual bool shouldCreatePlacemark(StarLink* link);  // 判断是否应创建地标
		virtual std::string createDescriptionString(StarLink* link);  // 创建描述字符串

	private:
		const std::vector<FPType>& flowDiff_;  // 链路流量差异向量
		FPType scaleFactor_;  // 缩放因子
		const FPType tolerance_;  // 容差值
		static const int maxLength_ = 10;  // 最大长度常量

		/** 计算链路宽度的缩放因子。该值取决于最大绝对链路流量差异。
		*/
		void calcScalingFactor();  // 计算缩放因子

};
#endif