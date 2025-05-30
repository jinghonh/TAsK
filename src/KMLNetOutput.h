#ifndef KML_NET_OUTPUT  // 防止头文件重复包含的宏定义开始
#define KML_NET_OUTPUT  // 定义宏

#include "NetOutput.h"  // 包含网络输出基类头文件

#include <limits>  // 包含数值限制相关的头文件

class StarLink;  // 前向声明StarLink类

/** \brief 基于流量/容量比率的网络性能分析KML（使用颜色编码）
    
    该类创建包含当前流量解决方案的kml文件。不同的颜色
    对应不同的拥堵程度（基于流量/容量比率）
*/
class KMLNetOutput : public NetOutput {  // 定义KML网络输出类，继承自NetOutput
	public:
		KMLNetOutput(StarNetwork* net);  // 构造函数，参数为网络指针
		KMLNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs);  // 带日志文件的构造函数
		virtual ~KMLNetOutput();  // 虚析构函数

		/** 创建kml文件
			@param fileWithNodes 包含每个节点坐标的文件路径（所有
				坐标必须使用wgs84坐标系统）
			@param kmlFileName kml文件写入路径
		*/
		void createKML(const std::string& fileWithNodes, const std::string& kmlFileName);

	protected:
		/** @return 流量/容量比率，如果容量为零则返回-1
		*/
		virtual FPType calcLineWidth(StarLink* link);  // 计算线宽度的虚函数
		/** @return 根据线宽度返回kml颜色：如果lineWidth为-1，返回绿色，
				如果lineWidth大于1，返回红色，否则返回蓝色
		*/
		virtual std::string createColorFromLineWidth(FPType lineWidth);  // 根据线宽度创建颜色的虚函数
		/** @return 用于kml输出的缩放线宽度
		*/
		virtual std::string createLineWidth(FPType lineWidth);  // 创建线宽度的虚函数
		/** @return 如果链路流量为正则返回true，否则返回false
		*/
		virtual bool shouldCreatePlacemark(StarLink* link);  // 判断是否创建地标的虚函数
		/** @return 链路描述字符串
		*/
		virtual std::string createDescriptionString(StarLink* link);  // 创建描述字符串的虚函数
		/** @return 描述中使用的链路名称
		*/
		virtual std::string createLinkNameForDescription(StarLink* link);  // 创建链路名称的虚函数

	private:
		std::string createKmlHeader();  // 创建KML头部的私有方法
		std::string createKmlFooter();  // 创建KML尾部的私有方法
		std::string createPlacemark(FPType x1, FPType y1, FPType x2,
			FPType y2, StarLink* link);  // 创建地标的私有方法
		std::string createStyleForLink(int linkIndex, FPType lineWidth);  // 创建链路样式的私有方法
		std::string createDescription(StarLink* link);  // 创建描述的私有方法
		std::string createCoordLine(FPType x1, FPType y1, 
			FPType x2 = std::numeric_limits<FPType>::infinity(), 
			FPType y2 = std::numeric_limits<FPType>::infinity());  // 创建坐标线的私有方法

};
#endif  // 防止头文件重复包含的宏定义结束