// 包含必要的头文件
#include "DefaultNetParser.h"
#include "UtilsString.h"
#include "Error.h"
#include "BprFnc.h"
#include "FileReader.h"
#include "StarNetwork.h"
#include "StarNode.h"
#include "StarLink.h"

#include <sstream>
#include <stdlib.h>

/** 内部参数结构体，用于存储网络参数
*/ 
struct ParamsLine{
    int origin;      // 起点
    int dest;        // 终点
    FPType capacity; // 容量
    FPType length;   // 长度
    FPType freeFlowTime; // 自由流时间
    FPType b;        // BPR函数参数b
    FPType power;    // BPR函数幂次
    FPType speed;    // 速度
    FPType toll;     // 收费
    int type;        // 类型
};

// 构造函数，初始化网络文件路径
DefaultNetParser::DefaultNetParser(const std::string& fileWithNetwork) : 
                fileWithNetwork_(fileWithNetwork) {
};

// 析构函数
DefaultNetParser::~DefaultNetParser(){
};

// 解析网络文件并创建网络对象
StarNetwork* DefaultNetParser::parseNetwork(){
    // 创建文件读取器
    FileReader readFile(fileWithNetwork_);
        
    std::string netName("");
    // 从文件路径中提取网络名称
    Utils::extractName(fileWithNetwork_.c_str(), netName);
    
    // 解析文件头数据
    DataFromHeader data = parseDataFromHeader(readFile);
    
    // 创建网络对象
    StarNetwork* net = new StarNetwork(data.nbNodes, data.nbLinks, netName);
    // 分配收费数组
    allocateTolls(data.nbLinks);

    char semic;          // 分号
    ParamsLine params;   // 参数结构体
    int originPrev = -1; // 前一个起点
    LinkFnc *fnc = NULL; // 链接函数
    bool isZone = false; // 是否为区域
    int nbZones = 0;     // 区域数量
    StarLink* link = NULL; // 链接对象

    std::string line("");

    // 逐行读取文件
    while (readFile.isGood()) { 
        line = readFile.getNextLine(); 
        // 跳过注释行
        line = Utils::skipOneLineComment("~", line);
        
        if (!Utils::deleteWhiteSpaces(line).empty()){
            // 解析每行数据
            std::stringstream strLine(line);
            strLine >> params.origin >> params.dest >> params.capacity >> params.length >> 
                        params.freeFlowTime >> params.b >> params.power >> params.speed >> 
                        params.toll >> params.type >> semic;
            // 处理新节点
            if (originPrev != params.origin) {
                // 判断是否为区域节点
                if (params.origin < data.firstNode) {
                    isZone = true;
                    ++nbZones;
                } else {
                    isZone = false;
                }
                // 添加节点
                net->addNode(new StarNode(params.origin, isZone));
                originPrev = params.origin;
            }
            
            // 创建链接函数和链接对象
            fnc = new BprFnc(params.freeFlowTime, params.b, params.capacity, params.power);
            link = new StarLink(params.origin, params.dest, fnc);
            net->addLink(link);
            // 设置收费
            setToll(link->getIndex(), params.toll);
        }
    }
    return net;     
};

// 解析文件头数据
DataFromHeader DefaultNetParser::parseDataFromHeader(FileReader& readFile) {
    std::string line("");    // 当前行
    std::string field("");   // 字段名
    std::string value("");   // 字段值
    DataFromHeader data;     // 头数据
    data.nbNodes = 0;        // 节点数
    data.nbLinks = 0;        // 链接数
    data.firstNode = 0;      // 第一个节点
    // 读取直到元数据结束标记
    while (line.find("<END OF METADATA>") == std::string::npos) {
        line = readFile.getNextLine();
        line = Utils::skipOneLineComment("~", line);
        
        if (!Utils::deleteWhiteSpaces(line).empty()){
            // 提取字段名和值
            field = Utils::getSubString("<", ">", line);
            value = line.substr(line.find(">") + 1);
            // 解析不同类型的字段
            if (field == "NUMBER OF NODES"){
                data.nbNodes = atoi(value.c_str());
            } else if (field == "NUMBER OF LINKS") { 
                data.nbLinks = atoi(value.c_str());
            } else if (field == "FIRST THRU NODE") {
                data.firstNode = atoi(value.c_str());
            }
        }
        // 检查文件完整性
        if (!readFile.isGood()) {
            std::string message = "文件中缺少<END OF METADATA>标记: ";
            message.append(fileWithNetwork_);
            Error er(message);
            throw er;
        }
    }
    // 验证必要数据
    Utils::checkEmptyValue(data.nbNodes);
    Utils::checkEmptyValue(data.nbLinks);
    Utils::checkEmptyValue(data.firstNode); 
    return data;    
};