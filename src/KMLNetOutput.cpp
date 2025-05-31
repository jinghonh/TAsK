// 包含必要的头文件
#include "KMLNetOutput.h"
#include "StarNetwork.h"
#include "FileWriter.h"
#include "StarLink.h"

#include <iomanip>  // 用于格式化输出
#include <sstream>  // 用于字符串流处理
#include <iostream> // 用于标准输入输出

// 构造函数，初始化网络
KMLNetOutput::KMLNetOutput(StarNetwork* net) : NetOutput(net) {

};

// 带日志文件的构造函数
KMLNetOutput::KMLNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs) : 
			NetOutput(net, logFileForMissingIDs) {

};

// 析构函数
KMLNetOutput::~KMLNetOutput()= default;

// 创建KML文件的主函数
void KMLNetOutput::createKML(const std::string& fileWithNodes, const std::string& kmlFileName){
	// 输出KML文件路径
	std::cout << "KML将写入到: " << kmlFileName << std::endl;
	// 获取节点数量
	const int nbNodes = net_->getNbNodes();
	// 初始化坐标和节点ID数组
	std::vector<FPType> xCoord(nbNodes, 0);
	std::vector<FPType> yCoord(nbNodes, 0);
	std::vector<int> nodeID(nbNodes, 0);
	
	// 读取节点坐标
	readCoord(fileWithNodes, xCoord, yCoord, nodeID);

	// 创建KML文件写入器
	FileWriter writeKml(kmlFileName);
	writeKml.writeLine(createKmlHeader());
	
	// 遍历所有链接
	for (StarLink* link = net_->beginOnlyLink(); link != NULL; link = net_->getNextOnlyLink()) {
		if (shouldCreatePlacemark(link)) {
			// 获取链接的起点和终点索引
			int tail = link->getNodeFromIndex();
			int head = link->getNodeToIndex();
			// 获取坐标
			FPType x1 = xCoord[tail];
			FPType y1 = yCoord[tail];
			FPType x2 = xCoord[head];
			FPType y2 = yCoord[head];
			// 检查缺失的节点坐标
			if (x1 == 0 && y1 == 0) std::cout << "缺失节点坐标: " << link->getNodeFrom() << 
								" 链接: " << link->toString() << std::endl; 
			if (x2 == 0 && y2 == 0) std::cout << "缺失节点坐标: " << link->getNodeTo() << 
								" 链接: " << link->toString() << std::endl;
			// 如果坐标有效，创建地标
			if (x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0) 
				writeKml.writeLine(createPlacemark(x1, y1, x2, y2, link)); 
		}
	}

	// 写入KML文件尾部
	writeKml.writeLine(createKmlFooter());
};

// 判断是否应该为链接创建地标
bool KMLNetOutput::shouldCreatePlacemark(StarLink* link) {
	return link->getFlow() > 0.0;
};

// 创建KML文件头部
std::string KMLNetOutput::createKmlHeader() {
	std::string header;
	header.append("<?xml version=\"1.0\" encoding=\"utf-8\"?> \n");
	header.append("<kml xmlns=\"http://earth.google.com/kml/2.2\"> \n");
	header.append("\t<Document>\n");
    header.append("\t\t<name>交通网络: " + net_->getNetName() + "</name>\n"); 
 	header.append("\t\t<Style id=\"styleEmpty\">\n");
    header.append("\t\t\t<IconStyle>\n"); 
    header.append("\t\t\t\t<Icon></Icon>\n"); 
    header.append("\t\t\t</IconStyle>\n"); 
    header.append("\t\t\t<LabelStyle>\n"); 
    header.append("\t\t\t\t<scale>1.1</scale> \n");
    header.append("\t\t\t</LabelStyle> \n");
    header.append("</Style>\n");
    return header;
};

// 创建KML文件尾部
std::string KMLNetOutput::createKmlFooter(){
	return ("\t</Document>\n</kml>\n");
};

// 创建地标
std::string KMLNetOutput::createPlacemark(FPType x1, FPType y1, FPType x2,
		FPType y2, StarLink* link){ 
	std::stringstream ss;

	// 获取链接索引
	int linkIndex = link->getIndex();
	std::string placemark;
	// 添加链接样式
	placemark.append(createStyleForLink(linkIndex, calcLineWidth(link)));

	placemark.append("<Placemark>\n");
	
	// 添加描述信息
	placemark.append(createDescription(link));

	// 添加样式URL
	ss << "\t<styleUrl>#style" << linkIndex << "</styleUrl>\n";
	placemark.append(ss.str());
    placemark.append("\t\t<LineString>\n");
    placemark.append("\t\t\t" + createCoordLine(x1, y1, x2, y2)); 
    placemark.append("\t\t</LineString> \n");
    placemark.append("</Placemark> \n");

    // 添加点标记
    placemark.append("<Placemark>\n");
    ss.str("");
    placemark.append("\t<styleUrl>#styleEmpty</styleUrl>\n");
    placemark.append("<Point>\n");
    ss.str("");
    ss << "<coordinates> " << x1 << ", " << y1 << ", 0 </coordinates> \n";
    placemark.append(ss.str());
    placemark.append("</Point>\n");
	placemark.append("</Placemark> \n");    
    return placemark;
};

// 创建链接样式
std::string KMLNetOutput::createStyleForLink(int linkIndex, FPType lineWidth){
	std::stringstream ss;
	ss << "\t\t<Style id=\"style" << linkIndex << "\">\n";
	std::string styleString(ss.str());
    styleString.append("\t\t\t<IconStyle>\n"); 
    styleString.append("\t\t\t\t<Icon></Icon>\n"); 
    styleString.append("\t\t\t</IconStyle>\n"); 
    styleString.append("\t\t\t<LabelStyle>\n"); 
    styleString.append("\t\t\t\t<scale>1.1</scale> \n");
    styleString.append("\t\t\t</LabelStyle> \n");
   	styleString.append("\t\t\t<LineStyle> \n");

    // 添加颜色和宽度
    styleString.append("\t\t\t\t<color>" + createColorFromLineWidth(lineWidth) + "</color> \n");
    styleString.append("\t\t\t\t<width>" + createLineWidth(lineWidth) + "</width> \n");
   	
   	styleString.append("\t\t\t</LineStyle> \n");
    styleString.append("\t\t</Style> \n");

    return styleString;
};

// 根据线宽创建颜色
std::string KMLNetOutput::createColorFromLineWidth(FPType lineWidth){
	if (lineWidth > 1.0) {
    	return getRedColor();  // 流量大于容量时显示红色
    } else if (lineWidth == -1.0) {
    	return getGreenColor();  // 容量为0时显示绿色
    }
   	return getBlueColor();  // 其他情况显示蓝色
};

// 创建线宽
std::string KMLNetOutput::createLineWidth(FPType lineWidth){
	std::stringstream ss;
	if (lineWidth < 0.0) lineWidth  = 1.0;
    ss << 3*lineWidth;
    return ss.str();
}

// 计算线宽
FPType KMLNetOutput::calcLineWidth(StarLink* link){
	FPType capacity = (link->getLinkFnc())->getCapacity();
	FPType lineWidth = link->getFlow() / capacity;
	if (capacity == 0.0) {
		return -1;
	}
	return lineWidth;
};

// 创建描述信息
std::string KMLNetOutput::createDescription(StarLink* link) {
	std::string description("");
	description.append("\t<name>" + createLinkNameForDescription(link) + "</name>\n");
	description.append("\t<description>" + createDescriptionString(link) + "</description>\n");
	return description;
};

// 创建链接名称
std::string KMLNetOutput::createLinkNameForDescription(StarLink* link) {
	std::stringstream ss;
	ss << link->getIndex();
	return ss.str();
};

// 创建描述字符串
std::string KMLNetOutput::createDescriptionString(StarLink* link){
	LinkFnc* fnc = link->getLinkFnc();
	FPType ratio = 0;
	if (fnc->getCapacity() != 0)  ratio = link->getFlow() / fnc->getCapacity();
	std::stringstream ss;
	ss << fnc->toString() << ", flow: " << link->getFlow() << ", time: " << 
		link->getTime() << ", capacity: " << fnc->getCapacity() << 
		", flow/capacity = " << ratio
		 << " from: " << link->getNodeFrom() << ", to: " << link->getNodeTo();
	return ss.str();
};

std::string KMLNetOutput::createCoordLine(FPType x1, FPType y1, FPType x2, FPType y2) {
	std::stringstream ss;
	int precision = 10;
	ss << "<coordinates>" << std::setprecision(precision) << x1 << "," << 
    	std::setprecision(precision) << y1 << ",0"; 
    if (x2 != std::numeric_limits<FPType>::infinity()) 
    	ss << "\n" << std::setprecision(precision) << x2 << "," << 
    	std::setprecision(precision) << y2 << ",0";
    ss << "</coordinates> \n";
    return ss.str();
};