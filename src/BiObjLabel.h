#ifndef BI_OBJ_LABEL  // 防止头文件重复包含
#define BI_OBJ_LABEL

#include "UsedTypes.h"  // 包含自定义类型定义

#include <string>  // 包含字符串类

class StarLink;  // 前向声明StarLink类

/** \brief 实现双目标标签类。

    每个标签包含:
    - 旅行时间
    - 通行费
    - 所属节点索引
    - 指向该标签的链接
    - 生成当前标签的前一个标签
*/
class BiObjLabel {
    public:
        /**
            @param time 旅行时间
            @param toll 通行费
            @param owner 标签所属的节点索引
            @param prevLabel 生成当前标签的前一个标签
            @param prevLink 指向该标签的链接
        */
        BiObjLabel(FPType time, TollType toll, int owner, BiObjLabel* prevLabel, StarLink* prevLink);  // 构造函数
        ~BiObjLabel();  // 析构函数

        FPType getTime() const;  // 获取旅行时间

        TollType getToll() const;  // 获取通行费

        BiObjLabel* getPrevLabel() const;  // 获取前一个标签
        int getOwner() const;  // 获取所属节点索引

        StarLink* getPrevLink() const;  // 获取前一个链接

        std::string toString() const;  // 转换为字符串表示

        void setTime(FPType timeVal);  // 设置旅行时间

    private:
        FPType time_;  // 旅行时间
        const TollType toll_;  // 通行费(常量)
        const int owner_;  // 所属节点索引(常量)
        BiObjLabel* prevLabel_;  // 前一个标签指针
        StarLink* prevLink_;  // 前一个链接指针
};

#endif