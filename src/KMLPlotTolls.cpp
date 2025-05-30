// 包含必要的头文件
#include "KMLPlotTolls.h"  // 包含KML收费绘图类的头文件
#include "StarLink.h"      // 包含网络链路类的头文件

#include <sstream>         // 包含字符串流处理头文件

// 构造函数：初始化KML收费绘图对象
// 参数：net - 网络对象，tolls - 收费容器
KMLPlotTolls::KMLPlotTolls(StarNetwork* net, const TollContainerType& tolls) : 
		KMLNetOutput(net),    // 调用基类构造函数
		tolls_(tolls) {       // 初始化收费容器

};

// 带日志文件的构造函数
// 参数：net - 网络对象，logFileForMissingIDs - 缺失ID的日志文件，tolls - 收费容器
KMLPlotTolls::KMLPlotTolls(StarNetwork* net, const std::string& logFileForMissingIDs,
			const TollContainerType& tolls) :
			KMLNetOutput(net, logFileForMissingIDs),  // 调用基类构造函数
			tolls_(tolls) {                           // 初始化收费容器

};

// 析构函数
KMLPlotTolls::~KMLPlotTolls() {

};

// 计算线宽的方法
// 参数：link - 网络链路
// 返回：如果链路没有收费返回-1，否则返回0
FPType KMLPlotTolls::calcLineWidth(StarLink* link) {
	if (tolls_[link->getIndex()] == 0) return -1;  // 如果链路收费为0，返回-1
	return 0;                                       // 否则返回0
};

// 根据线宽创建颜色的方法
// 参数：lineWidth - 线宽值
// 返回：如果线宽小于0返回蓝色，否则返回红色
std::string KMLPlotTolls::createColorFromLineWidth(FPType lineWidth) {
	if (lineWidth < 0) return getBlueColor();  // 线宽小于0返回蓝色
	return getRedColor();                      // 否则返回红色
};

// 创建线宽的方法
// 参数：lineWidth - 线宽值
// 返回：固定返回"3"作为线宽
std::string KMLPlotTolls::createLineWidth(FPType lineWidth) {
	return "3";  // 返回固定线宽值
};

// 判断是否创建地标的方法
// 参数：link - 网络链路
// 返回：始终返回true，表示所有链路都创建地标
bool KMLPlotTolls::shouldCreatePlacemark(StarLink* link) {
	return true;  // 所有链路都创建地标
};

// 创建描述字符串的方法
// 参数：link - 网络链路
// 返回：包含链路收费信息的描述字符串
std::string KMLPlotTolls::createDescriptionString(StarLink* link) {
	std::stringstream ss;                                    // 创建字符串流
	ss << "Toll: " << tolls_[link->getIndex()];             // 将收费信息写入字符串流
	return ss.str();                                         // 返回字符串流内容
};
