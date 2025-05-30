// 包含必要的头文件
#include "IndiffCurveContainer.h"
#include "PieceWiseLinearIndiffCurve.h"
#include "Utils.h"
#include "ODMatrix.h"
#include "OneSourceBiObjShPath.h"
#include "Origin.h"
#include "FileWriter.h"
#include "PairOD.h"

#include <stdio.h>      // 标准输入输出
#include <stdlib.h>     // 标准库函数
#include <cassert>      // 断言
#include <algorithm>    // 算法库
#include <ctime>        // 时间相关
#include <iostream>     // 输入输出流
#include <math.h>       // 数学函数
#include <sstream>      // 字符串流
#include <limits>       // 数值限制

// 构造函数：初始化无差异曲线容器
IndiffCurveContainer::IndiffCurveContainer(int nbODpairs) : curves_(nbODpairs) {
    // 初始化所有曲线指针为NULL
    for (int i = 0; i < nbODpairs; ++i){
        curves_[i] = NULL;
    }
}; 

// 析构函数：释放所有曲线资源
IndiffCurveContainer::~IndiffCurveContainer() {
    deallocateCurves();
}; 

// 添加一条无差异曲线
void IndiffCurveContainer::addCurve(int odPairIndex, IndiffCurve* curve) {
    // 确保索引有效且该位置为空
    assert(odPairIndex >= 0 && static_cast<size_t>(odPairIndex) < curves_.size() 
                && curves_[odPairIndex] == NULL && curve != NULL);
    curves_[odPairIndex] = curve;
}; 

// 获取指定OD对的曲线
IndiffCurve* IndiffCurveContainer::getCurve(int odPairIndex) const {
    // 确保索引有效且该位置有曲线
    assert(odPairIndex >=0 && static_cast<size_t>(odPairIndex) < curves_.size() 
        && curves_[odPairIndex] != NULL);
    return curves_[odPairIndex];
}; 

// 使用最短路径生成随机曲线
void IndiffCurveContainer::generateRandomCurvesWithBSP(int maxNbPointsPerCurve, 
                OneSourceBiObjShPath& shPath,
                const ODMatrix& mat){
    deallocateCurves(); // 释放现有曲线
    // 遍历所有起点
    for (ConstOriginIterator it = mat.begin(); it != mat.end(); ++it){
        Origin* origin = *it;
        shPath.calculate(origin->getIndex());
        
        const BiObjLabelContainer& labels = shPath.getLabels();
        int maxNbLabels = 0;
        // 遍历所有终点
        for (PairODIterator destIt = origin->begin(); destIt != origin->end(); ++destIt) {
            PairOD* dest = *destIt;
            int maxToll = 0;
            FPType maxTime = 0;
            FPType minTime = std::numeric_limits<FPType>::infinity();
            int destIndex = dest->getIndex();
            // 更新最大标签数
            if (labels.getNbLabels(destIndex) > maxNbLabels)
                 maxNbLabels = labels.getNbLabels(destIndex);
            // 遍历所有标签
            for (LabelsIterator labelIt = labels.begin(destIndex);
                            labelIt != labels.end(destIndex); ++labelIt ) {
                BiObjLabel* label = *labelIt;
                // 更新最大收费
                if (label->getToll() > maxToll) {
                    maxToll = label->getToll();
                }
                // 更新最大时间
                if (label->getTime() > maxTime) {
                    maxTime = label->getTime();
                }
                // 更新最小时间
                if (label->getTime() < minTime) {
                    minTime = label->getTime();
                }
            }
            // 创建新的曲线
            curves_[dest->getODIndex()] = createCurve(std::min(maxNbPointsPerCurve, maxToll + 1), 
                maxToll + 1, 3*maxTime, minTime); 
        }
    }
};

// 生成随机曲线
void IndiffCurveContainer::generateRandomCurves(int maxNbPointsPerCurve, TollType maxToll,
                        FPType maxTime){
    deallocateCurves();
    // 为每个OD对创建随机曲线
    for (size_t i = 0; i < curves_.size(); ++i){
        curves_[i] = IndiffCurveContainer::createCurve(maxNbPointsPerCurve, maxToll, maxTime, 0);
    }
};

// 创建单条曲线
IndiffCurve* IndiffCurveContainer::createCurve(int maxNbPointsPerCurve, TollType maxToll,
                        FPType maxTime, FPType minTime){
    // 验证参数有效性
    assert(maxToll > 0 && maxNbPointsPerCurve <= maxToll && maxTime > 0);
    // 随机生成点数
    int nbPoints = rand() % maxNbPointsPerCurve + 1;
    std::vector<FPType> randomTimes(nbPoints);
    std::set<TollType> randomTolls;
    // 生成随机收费点
    generateRandomTolls(randomTolls, maxToll, nbPoints);
    // 生成随机时间点
    for (int i = 0; i < nbPoints; ++i) {
        randomTimes[i] =  Utils::generateRndNumber(maxTime); 
    }
    // 对时间点排序
    std::sort(randomTimes.begin(), randomTimes.end());
    // 创建分段线性无差异曲线
    PieceWiseLinearIndiffCurve* curve = new PieceWiseLinearIndiffCurve(nbPoints);
    std::set<TollType>::iterator it = randomTolls.begin();
    // 添加所有点
    for (int i = 0; i < nbPoints; ++i){
        assert(it != randomTolls.end());
        curve->addPoint(*it, randomTimes[i]);
        ++it;
    }
    return curve;
};

// 生成随机收费点
void IndiffCurveContainer::generateRandomTolls(std::set<TollType>& randomTolls,
                            TollType maxToll, int nbPoints) {
    randomTolls.insert(0); // 插入0收费点
    size_t size = nbPoints;
    // 生成随机收费点直到达到指定数量
    while (randomTolls.size() != size) {
        randomTolls.insert(rand() % maxToll + 1);
    }
};

// 释放所有曲线资源
void IndiffCurveContainer::deallocateCurves() {
    for (size_t i = 0; i < curves_.size(); ++i){
        delete curves_[i];
    }
}; 

// 打印所有曲线信息
void IndiffCurveContainer::print() const{
    for (size_t i = 0; i < curves_.size(); ++i){
        std::cout << "O-D pair " << i << " curve: " << (curves_[i])->toString() << std::endl;
    }
};

// 将曲线信息写入文件
void IndiffCurveContainer::writeToFile(const std::string& fileName, const ODMatrix& mat) const {
    FileWriter writer(fileName);
    writer.writeLine("// 起点索引    终点索引    OD索引    曲线   \n");
    // 遍历所有OD对
    for (ConstOriginIterator it = mat.begin(); it != mat.end(); ++it){
        Origin* origin = *it;
        for (PairODIterator destIt = origin->begin(); destIt != origin->end(); ++destIt) {
            std::stringstream ss;
            PairOD* dest = *destIt;
            // 写入OD对信息和对应的曲线
            ss << origin->getIndex() << " " << dest->getIndex() << " " << dest->getODIndex() << 
                " " << (curves_[dest->getODIndex()])->toString() << "\n";
            writer.writeLine(ss.str());
        }
    }
};