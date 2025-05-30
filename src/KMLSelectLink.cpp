// 包含必要的头文件
#include "KMLSelectLink.h"  // KML选择链路类的头文件
#include "PathSet.h"        // 路径集合类的头文件
#include "StarNetwork.h"    // 星形网络类的头文件
#include "Path.h"           // 路径类的头文件
#include "ODSet.h"          // OD集合类的头文件

#include <sstream>          // 字符串流处理头文件

// 构造函数：初始化KML选择链路对象
// 参数：net - 网络对象，paths - 路径集合
KMLSelectLink::KMLSelectLink(StarNetwork* net, const PathSet& paths) :
            KMLNetOutput(net), currentLiknIndex_(-1), paths_(paths), 
            coloredLinks_(net->getNbLinks(), ""),     // 初始化彩色链路数组
            linkDescriptions_(net->getNbLinks(), 0) { // 初始化链路描述数组
    initialiseColoredLinks();  // 初始化彩色链路
};

// 带日志文件的构造函数
// 参数：net - 网络对象，paths - 路径集合，logFileForMissingIDs - 缺失ID的日志文件
KMLSelectLink::KMLSelectLink(StarNetwork* net, const PathSet& paths, 
                    const std::string& logFileForMissingIDs) :
            KMLNetOutput(net, logFileForMissingIDs), currentLiknIndex_(-1), paths_(paths),
            coloredLinks_(net->getNbLinks(), ""),     // 初始化彩色链路数组
            linkDescriptions_(net->getNbLinks(), 0) { // 初始化链路描述数组
    initialiseColoredLinks();  // 初始化彩色链路
};

// 析构函数
KMLSelectLink::~KMLSelectLink() {

};

// 初始化彩色链路的方法
void KMLSelectLink::initialiseColoredLinks() {
    for (size_t i = 0; i < coloredLinks_.size(); ++i) {
        coloredLinks_[i] = getBlueColor();  // 将所有链路初始化为蓝色
    }
};

// 设置链路索引的方法
// 参数：linkIndex - 链路索引
void KMLSelectLink::setLinkIndex(int linkIndex) {
    assert(linkIndex >= 0);  // 确保链路索引非负
    currentLiknIndex_ = linkIndex;  // 设置当前链路索引
    colorLinks();  // 为链路着色
};

// 计算线宽的方法
// 参数：link - 网络链路
// 返回：链路索引，用于定义链路颜色
FPType KMLSelectLink::calcLineWidth(StarLink* link) {
    return link->getIndex();  // 链路索引将定义链路颜色
};

// 根据线宽创建颜色的方法
// 参数：lineWidth - 线宽值
// 返回：对应链路的颜色
std::string KMLSelectLink::createColorFromLineWidth(FPType lineWidth) {
    return coloredLinks_[static_cast<int>(lineWidth)];  // 返回链路的颜色
};

// 创建线宽的方法
// 参数：lineWidth - 线宽值
// 返回：如果链路为红色返回"5"，否则返回"3"
std::string KMLSelectLink::createLineWidth(FPType lineWidth) { 
    if (coloredLinks_[static_cast<int>(lineWidth)] == getRedColor()) return "5";  // 红色链路使用更粗的线宽
    return "3";  // 其他链路使用标准线宽
};

// 判断是否创建地标的方法
// 参数：link - 网络链路
// 返回：始终返回true
bool KMLSelectLink::shouldCreatePlacemark(StarLink* link) {
    return true;  // 为所有链路创建地标
};

// 创建描述字符串的方法
// 参数：link - 网络链路
// 返回：包含链路在路径中出现次数的描述字符串
std::string KMLSelectLink::createDescriptionString(StarLink* link) {
    std::stringstream ss;
    ss << "is present in " << linkDescriptions_[link->getIndex()] << " paths";  // 创建描述字符串
    return ss.str();
};

// 为链路着色的方法
void KMLSelectLink::colorLinks() {
    std::stringstream ss;
    // 遍历所有OD集合
    for (ConstODSetIterator odsetIt = paths_.begin(); odsetIt != paths_.end(); ++odsetIt) {
        ODSet* odset = *odsetIt;
        // 遍历OD集合中的所有路径
        for (PathIterator pathIt = odset->begin(); pathIt != odset->end(); ++pathIt) {
            Path* path = *pathIt;
            if (pathHasSelectedLink(path)) {  // 如果路径包含选中的链路
                colorLinksOfPath(path, ss.str());  // 为路径上的链路着色
            }
        }
    }
};

// 检查路径是否包含选中链路的方法
// 参数：path - 路径对象
// 返回：如果路径包含选中链路返回true，否则返回false
bool KMLSelectLink::pathHasSelectedLink(Path* path) const {
    for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
        if ((*it)->getIndex() == currentLiknIndex_) {  // 检查链路索引是否匹配
            return true;
        }
    }
    return false;
};

// 为路径上的链路着色的方法
// 参数：path - 路径对象，decription - 描述字符串
void KMLSelectLink::colorLinksOfPath(Path* path, const std::string& decription) {
    for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
        StarLink* link = *it;
        coloredLinks_[link->getIndex()] = getRedColor();  // 将链路设置为红色
        ++linkDescriptions_[link->getIndex()];  // 增加链路描述计数
    }
};
