#ifndef NET_OUTPUT  // 防止头文件重复包含
#define NET_OUTPUT

class StarNetwork;  // 前向声明StarNetwork类

#include "UsedTypes.h"  // 包含自定义类型定义

#include <vector>  // 包含vector容器
#include <string>  // 包含string类

/** \brief 这是一个基类，用于对StarNetwork对象执行输出操作
    (创建KML、创建LaTeX等)
    \details 它包含了派生类使用的基本功能
*/
class NetOutput {
    public:
        virtual ~NetOutput();  // 虚析构函数

        // 构造函数，接收网络指针和日志文件名参数
        NetOutput(StarNetwork* net, const std::string& logFileForMissingIDs = "log.missing.ids.txt");
        
        /** 从文件中读取坐标，并将结果写入xCoord、yCoord和nodeID
        */
        void readCoord(const std::string& file, std::vector<FPType>& xCoord, 
                std::vector<FPType>& yCoord, std::vector<int>& nodeID);

    protected:
        StarNetwork* net_;  // 网络对象指针
        std::string logFileForMissingIDs_;  // 缺失ID的日志文件名

        // 注意：这些是KML颜色值，不是十六进制
        static const std::string getRedColor() {  // 获取红色
            return "7f0000ff";
        };

        static const std::string getBlueColor() {  // 获取蓝色
            return "50f04614";
        };

        static const std::string getGreenColor() {  // 获取绿色
            return "5014b41e";
        };

        static const std::string getLightBlueColor(){  // 获取浅蓝色
            return "50F0C814";
        };

        static const std::string getOrangeColor(){  // 获取橙色
            return "501478FF";
        };        

};

#endif