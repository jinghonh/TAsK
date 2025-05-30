#include "PieceWiseLinearIndiffCurve.h" // 包含头文件
#include "Error.h" // 包含错误处理类

#include <cassert> // 包含断言
#include <sstream> // 包含字符串流
#include <algorithm> // 包含算法库函数

PieceWiseLinearIndiffCurve::PieceWiseLinearIndiffCurve(int nbPoints) : nbPoints_(nbPoints), 
		size_(0), values_(nbPoints, 0), tolls_(nbPoints, 0), valuesCopy_(nbPoints, 0),
		tollsCopy_(nbPoints, 0) { // 构造函数，初始化所有成员变量
	assert(nbPoints > 0); // 断言点数大于0
}; 

PieceWiseLinearIndiffCurve::~PieceWiseLinearIndiffCurve() { // 析构函数
}; 

int PieceWiseLinearIndiffCurve::getNbPoints() const{ // 获取点数的方法
	return size_; // 返回当前大小
};

void PieceWiseLinearIndiffCurve::addPoint(TollType toll, FPType value) { // 添加点的方法
	assert(size_ < nbPoints_); // 断言当前大小小于总点数
	if (toll < 0 || value < 0) throw Error("无差异曲线的所有值必须非负"); // 检查负值
	if (size_ == 0 && toll != 0) throw Error("toll的第一个值必须为零"); // 检查第一个点
	if (size_ > 0 && (toll <= tolls_[size_ - 1] || value < values_[size_ - 1])) 
		throw Error ("尝试添加小于或等于前一个toll的值"); // 检查顺序
	tolls_[size_] = toll; // 设置toll值
	values_[size_] = value; // 设置value值
	tollsCopy_[size_] = toll; // 设置toll副本
	valuesCopy_[size_] = value; // 设置value副本
	++size_; // 增加大小计数
}; 

FPType PieceWiseLinearIndiffCurve::evaluate(TollType toll) { // 评估函数
	assert(toll >= 0); // 断言toll非负
	if (toll == 0) return values_[0]; // 如果toll为0，返回第一个值
	int i = 0; // 初始化索引
	while (toll - 1e-10 > tolls_[i]) { // 查找适当的区间
		++i; // 增加索引
		// toll大于最后添加的点
		if (i == nbPoints_) { // 如果到达点数上限
			return toll - tolls_[i - 1] + values_[i - 1]; // 返回计算结果
		}
	}
	return values_[i - 1] + (toll - tolls_[i - 1]) * (values_[i] - values_[i - 1]) / 
				(tolls_[i] - tolls_[i - 1]); // 返回线性插值结果
}; 

std::string PieceWiseLinearIndiffCurve::toString() { // 转为字符串的方法
	std::stringstream retVal(""); // 创建字符串流
	for (int i = 0; i < size_; ++i) { // 遍历所有点
		retVal << "(" << tolls_[i] << ", " << values_[i] << ") "; // 格式化点的信息
	}
	return retVal.str(); // 返回字符串
}; 

template<typename T> // 模板函数，用于扰动向量
void perturb(FPType percentage, FPType probOfPerturbation, std::vector<T>& container, int size) {
    assert(percentage > 0 && percentage <= 100 && 
		probOfPerturbation > 0  && probOfPerturbation <= 100); // 断言参数有效
	FPType bound = 0.0; // 初始化边界值
	FPType perturbation = 0.0; // 初始化扰动值
	
	for (int index = 0; index < size; ++index) { // 遍历所有元素
		if (rand() % 101 < probOfPerturbation) { // 根据概率决定是否扰动
			bound = container[index] * percentage / 100.0; // 计算边界
			perturbation = -bound + static_cast <FPType> (rand()) /
				( static_cast <FPType> (RAND_MAX/(2 * bound))); // 计算随机扰动
			container[index] += perturbation; // 应用扰动
		}
	}

	std::sort(container.begin(), container.end()); // 对容器排序
}

void PieceWiseLinearIndiffCurve::perturbTime(FPType percentage, FPType probOfPerturbation){ // 扰动时间的方法
	perturb(percentage, probOfPerturbation, values_, size_); // 调用perturb函数扰动values_
};

void PieceWiseLinearIndiffCurve::perturbToll(FPType percentage, FPType probOfPerturbation) { // 扰动通行费的方法
	perturb(percentage, probOfPerturbation, tolls_, size_); // 调用perturb函数扰动tolls_
	std::sort(values_.begin(), values_.end()); // 对values_排序
};

void PieceWiseLinearIndiffCurve::restoreOriginalCurve(){ // 恢复原始曲线的方法
	for (int i = 0; i < size_; ++i) { // 遍历所有点
		tolls_[i] = tollsCopy_[i]; // 从副本恢复toll值
		values_[i] = valuesCopy_[i]; // 从副本恢复value值
	}
}; 