// 包含必要的头文件
#include "LatexNetOutput.h"
#include "FileWriter.h"
#include "StarNetwork.h"
#include "StarNode.h"
#include "StarLink.h"

#include <sstream>
#include <math.h>
#include <iostream>

// 定义坐标容差值 - 增大容差以适应归一化后的坐标
#define COORD_TOL 0.1

// 构造函数1：初始化网络输出对象
LatexNetOutput::LatexNetOutput(StarNetwork* net) : NetOutput(net) {
	setPlotInfoToDefault();
};

// 构造函数2：初始化网络输出对象，包含缺失ID的日志文件
LatexNetOutput::LatexNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs) : 
			NetOutput(net, logFileForMissingIDs) {
	setPlotInfoToDefault();
};

// 析构函数
LatexNetOutput::~LatexNetOutput(){

};

// 设置默认的绘图信息
void LatexNetOutput::setPlotInfoToDefault(){
	snInfo_.side1 = "south";  // 设置南北方向的第一个边
	snInfo_.side2 = "north";  // 设置南北方向的第二个边
	snInfo_.shift = "xshift"; // 设置南北方向的偏移类型

	weInfo_.side1 = "west";   // 设置东西方向的第一个边
	weInfo_.side2 = "east";   // 设置东西方向的第二个边
	weInfo_.shift = "yshift"; // 设置东西方向的偏移类型
};

// 将网络输出为LaTeX格式
void LatexNetOutput::printToLaTeX(const std::string& fileWithNodes, const std::string& texFile, 
		bool addHeader, FPType scale) { 
	FileWriter outputFile(texFile);  // 创建输出文件对象
	std::cout << "file name: " << texFile << std::endl;
	
	// 如果需要添加LaTeX文档头
	if (addHeader) {
		outputFile.writeLine("\\documentclass[a4paper]{article} \n");
		outputFile.writeLine("\\usepackage{a4wide} \n");
		outputFile.writeLine("\\usepackage{tikz} \n \\usetikzlibrary{calc} \n");
		outputFile.writeLine("\\begin{document} \n");
	}
	// 创建tikz图形环境
	std::stringstream ss;
	ss << " \\begin{tikzpicture}[scale=" << scale << "] \n";
	outputFile.writeLine(ss.str());
	
	// 获取节点数量并初始化坐标数组
	int nbNodes = net_->getNbNodes();
	std::vector<FPType> xCoord(nbNodes, 0);
	std::vector<FPType> yCoord(nbNodes, 0);
	std::vector<int> nodeID(nbNodes, 0);
	
	// 读取节点坐标
	readCoord(fileWithNodes, xCoord, yCoord, nodeID);

	// 添加TikZ节点
	addTikzNodes(outputFile, xCoord, yCoord, nodeID);

	// 获取流量信息
	FlowInfo flowInfo = getFlowInfo();
	
	// 遍历所有节点和链接
	for (StarNode* node = net_->beginNode(); node != NULL; node = net_->getNextNode()){
		for (StarLink* link = net_->beginLink(); link != NULL; link = net_->getNextLink()) {
			
			if (plotLink(link)) {  // 如果链接需要绘制
				int tail = node->getIndex();  // 获取起始节点索引
				int head = link->getNodeToIndex();  // 获取目标节点索引
				std::string lineOptions = generateLineOptions(flowInfo, link);  // 生成线条选项
				
				std::string oneLine("");
				setPlotInfoToDefault();
				// 根据链接方向选择绘制方法
				if (onHorizontalLine(tail, head, yCoord)) {
					oneLine = drawHorizontalLink(link, lineOptions, (xCoord[tail] < xCoord[head]));
				} else if (onVerticalLine(tail, head, xCoord)) {
					oneLine = drawVerticalLink(link, lineOptions, (yCoord[tail] < yCoord[head]));
				} else {
					oneLine = drawDiagonalLink(link, lineOptions, (xCoord[tail] < xCoord[head]),
						(yCoord[tail] < yCoord[head]));
				}
				if (!oneLine.empty()) outputFile.writeLine(oneLine);
			}
		} 
	}	
	
	// 结束tikz图形环境
	outputFile.writeLine(" \\end{tikzpicture} \n \n");
	
	// 如果需要，添加文档结束标记
	if (addHeader) outputFile.writeLine("\\end{document} \n");
};

// 判断是否需要绘制链接
bool LatexNetOutput::plotLink(StarLink* link) const {
	return link->getFlow() > 0.0;
};

// 添加TikZ节点
void LatexNetOutput::addTikzNodes(FileWriter& outputFile, const std::vector<FPType>& xCoord, 
				const std::vector<FPType>& yCoord, const std::vector<int>& nodeID){
	int nbNodes = net_->getNbNodes();
	for (int i = 0; i < nbNodes; ++i) {
		// 检查节点坐标是否有效，允许0坐标值，只要不是-1
		if (xCoord[i] != -1 && yCoord[i] != -1) {  
			std::stringstream tmpss;
			// 创建节点绘制命令 - 减小节点尺寸以适应归一化坐标
			tmpss << "\\node[draw,thick,circle,black,minimum size=0.2cm] (n" << nodeID[i] << ") at (" << xCoord[i] << "," 
				<< yCoord[i] << ") {" << getNodeLabel(nodeID[i]) << "}; \n";
			outputFile.writeLine(tmpss.str());
		}
	}
};

// 获取节点标签
std::string LatexNetOutput::getNodeLabel(int nodeID) const {
	std::stringstream ss;
	ss << nodeID;
	return ss.str();
};

// 判断是否在水平线上
bool LatexNetOutput::onHorizontalLine(int tail, int head, const std::vector<FPType>& yCoord){
	return (fabs(yCoord[tail] - yCoord[head]) <= COORD_TOL); 
};

// 判断是否在垂直线上
bool LatexNetOutput::onVerticalLine(int tail, int head, const std::vector<FPType>& xCoord){
	return (fabs(xCoord[tail] - xCoord[head]) <= COORD_TOL); 
};

// 获取线条标签
std::string LatexNetOutput::getLineLabel(StarLink* link, const std::string& aboveOrBelow){
	std::stringstream ss;
	ss << "-- node[sloped," + aboveOrBelow + "] {" <<  
			link->getTime() << "}";
	return ss.str();
};

// 绘制水平链接
std::string LatexNetOutput::drawHorizontalLink(StarLink* link, const std::string& lineOptions, 
		bool tailIsLeft){
	
	std::stringstream ss;
	std::string aboveOrBelow("below");
	if (tailIsLeft) aboveOrBelow = "above";
	// 使用简化的连接方式
	ss << "\\draw[" << lineOptions <<"] (n" << link->getNodeFrom() << ") " 
					<< getLineLabel(link, aboveOrBelow) << " (n" << 
					link->getNodeTo() << "); \n";
	return ss.str();
};

// 绘制垂直链接
std::string LatexNetOutput::drawVerticalLink(StarLink* link, const std::string& lineOptions, 
		bool tailIsUp){
	std::stringstream ss;
	// 使用简化的连接方式
	ss << "\\draw[" << lineOptions <<"] (n" << link->getNodeFrom() << ") " 
					<< getLineLabel(link, "above") << " (n" << 
					link->getNodeTo() << "); \n";
	return ss.str();
};

std::string LatexNetOutput::drawDiagonalLink(StarLink* link, const std::string& lineOptions,
 		const bool tailIsLeft, bool tailIsUp){
 	std::stringstream ss;
 	std::string aboveOrBelow("below");
	if (tailIsLeft) aboveOrBelow = "above";
	// 使用简化的连接方式
 	ss << "\\draw[" << lineOptions <<"] (n" << link->getNodeFrom() << ") " 
					<< getLineLabel(link, aboveOrBelow) << " (n" << 
					link->getNodeTo() << "); \n";
 	return ss.str();
 };

 FlowInfo LatexNetOutput::getFlowInfo(){
	FPType totalFlow = 0.0;
	FPType minFlow = std::numeric_limits<FPType>::max( );
	FPType maxFlow = 0.0;
	for (StarLink* link = net_->beginOnlyLink(); link != NULL; link = net_->getNextOnlyLink()) {
		FPType flow = link->getFlow();
		if (flow < minFlow) {
			minFlow = flow;
		}
		if (flow > maxFlow) {
			maxFlow = flow;
		}
		totalFlow += flow;
	}
	FlowInfo flowInfo;
	flowInfo.minFlow = minFlow;
	flowInfo.maxFlow = maxFlow;
	flowInfo.totalFlow = totalFlow;
	return flowInfo;
};

std::string LatexNetOutput::generateLineOptions(const FlowInfo& flowInfo, StarLink* link){
	std::stringstream sstm;
	FPType tmpFlow = (link->getFlow() - flowInfo.minFlow) / (flowInfo.maxFlow - flowInfo.minFlow);
	FPType capacity = (link->getLinkFnc())->getCapacity();
	std::string color("blue");
	if (link->getFlow() / capacity > 1) {
		color = "red";
	}
	// 减小线宽计算以适应归一化坐标
	double lineWidth = 0.3 + 1.2 * tmpFlow;
	sstm << "line width=" << lineWidth << ", " << color << ", ->";
	return sstm.str();
};

int LatexNetOutput::getShift(PlotInfo& node, bool swapValues){
	// 减小偏移值以适应归一化坐标
	int shiftVal = 0.5;
	if (swapValues) {
		std::swap(node.side1, node.side2);
		return -shiftVal;
	}
	return shiftVal;
}

std::string LatexNetOutput::createShiftStr(const std::string& shift, int val){
	std::stringstream ss;
	ss << shift << "=" << val << "pt";
	return ss.str();
};