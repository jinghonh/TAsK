#ifndef LINK_FCN  // 防止头文件重复包含
#define LINK_FCN

#include "UsedTypes.h"  // 包含类型定义头文件
#include "Constants.h"  // 包含常量定义头文件

#include <string>  // 包含字符串处理库

/** \brief 用于表示链路成本函数的抽象类
    \details 任何具体的链路成本函数类必须继承这个类并实现其虚方法
 */
class LinkFnc {
	
	public:
	
		virtual ~LinkFnc(){};  // 虚析构函数
		
		/** 计算给定流量下的链路成本函数值 */
		virtual FPType evaluate(FPType linkFlow) const =0;  // 纯虚函数，计算链路成本

		/** 计算给定流量下链路成本函数的一阶导数 */
		virtual FPType evaluateDerivative(FPType linkFlow) const =0;  // 纯虚函数，计算一阶导数

		/** 计算给定流量下链路成本函数的二阶导数 */
		virtual FPType evaluateSecondDerivative(FPType linkFlow) const { return 0.0; };  // 计算二阶导数，默认返回0
		
		/** 在屏幕上打印链路成本函数的参数 */
		virtual void print() const =0;  // 纯虚函数，打印参数

		/** @return 返回描述链路成本函数参数的字符串 */
		virtual std::string toString() const =0;  // 纯虚函数，转换为字符串
		
		/** 创建链路成本函数的副本 */
		virtual LinkFnc * clone() const =0;  // 纯虚函数，克隆对象

		/** 同时计算函数值及其导数
			\note 导数值写入输入参数der中
			@return 返回计算得到的函数值
		*/
		virtual FPType evaluateTimeAndDerivative(FPType linkFlow, FPType &der){  // 计算时间和导数
			der = evaluateDerivative(linkFlow);  // 计算导数
			return evaluate(linkFlow);  // 返回函数值
		};

		/** @return 如果未重写则返回0.0 */
		virtual FPType getCapacity() {  // 获取容量
			return 0.0;
		};

		/** 此方法仅在SpiessFnc、SpiessFncComb、PlusLinearFnc中实现
			@return 如果未重写则返回0.0
		*/
		virtual FPType getLength() const {  // 获取长度
			return 0.0;
		};

		/** 此方法仅在SpiessFnc、SpiessFncComb、PlusLinearFnc中实现
			如果未重写则不执行任何操作
		*/
		virtual void setLength(FPType length) {  // 设置长度

		};

		/** @return 返回函数类型。所有类型都列在Constants.h中 */
		virtual FncType getType() = 0;  // 纯虚函数，获取函数类型
	
	protected:
		LinkFnc(){};  // 受保护的构造函数
};

#endif
