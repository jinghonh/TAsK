#ifndef KML_PLOT_TOLLS  // 防止头文件重复包含
#define KML_PLOT_TOLLS 

#include "KMLNetOutput.h"  // 包含KML网络输出基类

/** \brief 该类创建一个KML文件，其中收费路段以红色高亮显示。
*/
class KMLPlotTolls : public KMLNetOutput {  // 继承自KMLNetOutput类
	public:
		// 构造函数，接收网络和收费信息
		KMLPlotTolls(StarNetwork* net, const TollContainerType& tolls);
		// 构造函数，额外接收缺失ID的日志文件
		KMLPlotTolls(StarNetwork* net, const std::string& logFileForMissingIDs,
			const TollContainerType& tolls);
		// 析构函数
		~KMLPlotTolls();

	protected:
		/** @return 如果收费为0返回-1，否则返回0
		*/
		virtual FPType calcLineWidth(StarLink* link);
		/** @return 如果lineWidth < 0返回蓝色，否则返回红色
		*/
		virtual std::string createColorFromLineWidth(FPType lineWidth);
		/** @return 返回"3"
		*/
		virtual std::string createLineWidth(FPType lineWidth);
		/** @return 返回true
		*/
		virtual bool shouldCreatePlacemark(StarLink* link);
		// 创建描述字符串
		virtual std::string createDescriptionString(StarLink* link);

	private:
		const TollContainerType& tolls_;  // 存储收费信息的容器引用

};
#endif