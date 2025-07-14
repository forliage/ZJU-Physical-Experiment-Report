#pragma once

#include "DataSet.h"
#include <vector>
#include <eigen/Eigen/Dense>

namespace Calculator {

    struct LinearFitResult {
        double slope_a = 0.0;
        double intercept_b = 0.0;
        double r_squared = 0.0;
    };
    
    struct StatisticsResult {
        double mean = 0.0;                 // 平均值
        double std_dev = 0.0;              // 标准偏差 (样本)
        double variance = 0.0;             // 方差 (样本)
        double cv = 0.0;                   // 变异系数
        size_t count = 0;                  // 数据点数量
    };

    StatisticsResult calculateStatistics(const std::vector<double>& data);

    double calculateUncertaintyA(const DataSet& dataSet);
    LinearFitResult linearFit(const DataSet& dataSet);
    Eigen::VectorXd polynomialFit(const DataSet& dataSet, int order);
    double combineUncertainties(const std::vector<double>& uncertainties);
    double calculateTotalUncertainty(double uncertainty_a, double uncertainty_b);

    // 计算多项式曲线在给定区间的定积分（面积）
    // @param coeffs: 多项式系数向量 (从 c0 到 cn)
    // @param x_start: 积分起始点
    // @param x_end: 积分结束点
    // @return: 曲线下的面积
    double calculateAreaUnderCurve(const Eigen::VectorXd& coeffs, double x_start, double x_end);

} // namespace Calculator