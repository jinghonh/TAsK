#ifndef LATEX_NET_OUTPUT  // 防止头文件重复包含
#define LATEX_NET_OUTPUT

#include "NetOutput.h"  // 包含基类头文件

class StarLink;  // 前向声明StarLink类
class FileWriter;  // 前向声明FileWriter类

struct FlowInfo {  // 定义流量信息结构体
	FPType minFlow;  // 最小流量
	FPType maxFlow;  // 最大流量
	FPType totalFlow;  // 总流量
};

struct PlotInfo {  // 定义绘图信息结构体
	std::string side1;  // 第一边信息
	std::string side2;  // 第二边信息
	std::string shift;  // 偏移信息
};

/** \brief 该类负责创建用于绘制链路流的latex文件。
    为了创建latex文件，必须知道节点坐标。

    该算法绘制网络性能图，即拥堵链路显示为红色，
    非拥堵链路显示为蓝色，每条链路的宽度与流量/容量比成正比。
    只绘制具有正流量的链路。
*/
class LatexNetOutput : public NetOutput {  // 继承自NetOutput类
	public:
		LatexNetOutput(StarNetwork* net);  // 构造函数
		LatexNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs);  // 带日志文件的构造函数
		~LatexNetOutput();  // 析构函数

		/** @param fileWithNodes 包含节点坐标的文件路径
			@param texFile 写入latex代码的文件路径
			@param addHeader 如果为true则添加latex头文件，如果为false则只写入tikzpicture代码
			@param scale \begin{tikzpicture}[scale = scale]中的缩放比例
		*/
		void printToLaTeX(const std::string& fileWithNodes, const std::string& texFile, 
			bool addHeader, FPType scale);

	protected:
		/** @return 描述链路行程时间的线标签
		*/
		virtual std::string getLineLabel(StarLink* link, const std::string& aboveOrBelow);
		/** @return 给定链路的选项，如线宽和颜色
		*/
		virtual std::string generateLineOptions(const FlowInfo& flowInfo, StarLink* link);
		/** @return 如果链路流量为正则返回true，否则返回false
		*/
		virtual bool plotLink(StarLink* link) const;
		/** @return 节点ID
		*/
		virtual std::string getNodeLabel(int nodeID) const;

	private:
		PlotInfo snInfo_;  // 南北方向绘图信息
		PlotInfo weInfo_;  // 东西方向绘图信息

		void addTikzNodes(FileWriter& outputFile, const std::vector<FPType>& xCoord, const std::vector<FPType>& yCoord, 
				const std::vector<int>& nodeID);  // 添加TikZ节点

		void setPlotInfoToDefault();  // 设置默认绘图信息
 		FlowInfo getFlowInfo();  // 获取流量信息
		
		bool onHorizontalLine(int tail, int head, const std::vector<FPType>& yCoord);  // 判断是否在水平线上
		bool onVerticalLine(int tail, int head, const std::vector<FPType>& xCoord);  // 判断是否在垂直线上
		std::string drawHorizontalLink(StarLink* link, const std::string& lineOptions, 
			bool tailIsLeft);  // 绘制水平链路
		std::string drawVerticalLink(StarLink* link, const std::string& lineOptions, 
 			bool tailIsUp);  // 绘制垂直链路
		std::string drawDiagonalLink(StarLink* link, const std::string& lineOptions, 
 			bool tailIsLeft, bool tailIsUp);  // 绘制对角线链路

		std::string createShiftStr(const std::string& shift, int val);  // 创建偏移字符串
		int getShift(PlotInfo& node, bool swapValues);  // 获取偏移值
		
};

#endif