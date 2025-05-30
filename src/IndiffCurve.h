#ifndef INDIFF_CURVE  // 防止头文件重复包含
#define INDIFF_CURVE

#include "UsedTypes.h"  // 包含自定义类型定义

#include <stdlib.h>  // 包含标准库
#include <string>    // 包含字符串类

/** \brief 表示两个目标非线性标量化函数的抽象类
    \details 标量化函数将通行费转换为时间。
    它必须是严格递增的（用于Larsson的优化公式 \cite Larsson2004）
*/
class IndiffCurve {
    public:
        // 虚析构函数
        virtual ~IndiffCurve(){};

        /** @return 在给定通行费下评估的标量化函数值 */
        virtual FPType evaluate(TollType toll) =0;

        /** @return 以字符串格式返回标量化函数的描述 */
        virtual std::string toString() =0;

        /** 默认不执行任何操作 */
        virtual void perturbTime(FPType percentage, FPType probOfPerturbation) {};
        /** 默认不执行任何操作 */
        virtual void perturbToll(FPType percentage, FPType probOfPerturbation) {};
        /** 默认不执行任何操作 */
        virtual void restoreOriginalCurve() {};

    protected:
        // 受保护的构造函数
        IndiffCurve(){};
};

#endif