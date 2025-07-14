#include "ReportGenerator.h"
#include <sstream> 
#include <iomanip>

std::string ReportGenerator::generateMarkdown(const ReportData& reportData) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(4); 

    ss << "# 大学物理实验报告\n\n";

    ss << "## 1. 原始数据\n\n";
    ss << "| X | Y |\n";
    ss << "|---|---|\n";
    for (const auto& p : reportData.data.getPoints()) {
        ss << "| " << p.x << " | " << p.y << " |\n";
    }
    ss << "\n";

    if (reportData.statsCalculated) {
        ss << "## 2. 统计分析\n\n";
        ss << "- 数据点数量 (N): " << reportData.stats.count << "\n";
        ss << "- 平均值: " << reportData.stats.mean << "\n";
        ss << "- 方差 (s²): " << reportData.stats.variance << "\n";
        ss << "- 标准偏差 (s): " << reportData.stats.std_dev << "\n";
        ss << "- 变异系数 (s/mean): " << reportData.stats.cv << "\n\n";
    }

    if (reportData.linearFitCalculated || reportData.polyFitCalculated) {
        ss << "## 3. 拟合结果\n\n";
    }
    if (reportData.linearFitCalculated) {
        ss << "### 线性拟合\n";
        ss << "- **方程**: `y = " << reportData.linearFit.slope_a << " * x + " << reportData.linearFit.intercept_b << "`\n";
        ss << "- **相关系数 (R²)**: " << reportData.linearFit.r_squared << "\n\n";
    }
    if (reportData.polyFitCalculated) {
        ss << "### " << reportData.polyOrder << "阶多项式拟合\n";
        ss << "- **方程**: `y = ";
        for (int i = reportData.polyCoeffs.size() - 1; i >= 0; --i) {
            if (i < reportData.polyCoeffs.size() - 1) ss << (reportData.polyCoeffs[i] >= 0 ? " + " : " - ");
            ss << std::abs(reportData.polyCoeffs[i]);
            if (i > 0) ss << " * x";
            if (i > 1) ss << "^" << i;
        }
        ss << "`\n\n";
    }

    if (reportData.uncertaintyCalculated) {
        ss << "## 4. 不确定度分析\n\n";
        ss << "- A类不确定度 (uA): " << reportData.uncertaintyA << "\n";
        ss << "- B类不确定度 (uB): " << reportData.uncertaintyB << "\n";
        ss << "- **总不确定度 (u)**: " << reportData.totalUncertainty << "\n\n";
    }

    return ss.str();
}

std::string ReportGenerator::generateTypst(const ReportData& reportData) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(4);

    ss << "#set text(font: \"New Computer Modern\")\n";
    ss << "#set-heading(numbering: \"1.1\")\n\n";
    ss << "= 大学物理实验报告\n\n";

    ss << "== 原始数据\n\n";
    ss << "#table(\n";
    ss << "  columns: (auto, auto),\n";
    ss << "  stroke: 0.5pt,\n";
    ss << "  [*X*], [*Y*],\n";
    for (const auto& p : reportData.data.getPoints()) {
        ss << "  [" << p.x << "], [" << p.y << "],\n";
    }
    ss << ")\n\n";

    if (reportData.statsCalculated) {
        ss << "== 统计分析\n\n";
        ss << "- 数据点数量 (N): " << reportData.stats.count << "\n";
        ss << "- 平均值: " << reportData.stats.mean << "\n";
        ss << "- 方差 ($s^2$): " << reportData.stats.variance << "\n";
        ss << "- 标准偏差 ($s$): " << reportData.stats.std_dev << "\n";
        ss << "- 变异系数 ($s$/mean): " << reportData.stats.cv << "\n\n";
    }

    if (reportData.linearFitCalculated || reportData.polyFitCalculated) {
        ss << "== 拟合结果\n\n";
    }
    if (reportData.linearFitCalculated) {
        ss << "=== 线性拟合\n";
        ss << "- *方程*: $ y = " << reportData.linearFit.slope_a << " x + " << reportData.linearFit.intercept_b << " $\n";
        ss << "- *相关系数* ($R^2$): " << reportData.linearFit.r_squared << "\n\n";
    }
    if (reportData.polyFitCalculated) {
        ss << "=== " << reportData.polyOrder << "阶多项式拟合\n";
        ss << "- *方程*: $ y = ";
        for (int i = reportData.polyCoeffs.size() - 1; i >= 0; --i) {
             if (i < reportData.polyCoeffs.size() - 1) ss << (reportData.polyCoeffs[i] >= 0 ? " + " : " - ");
            ss << std::abs(reportData.polyCoeffs[i]);
            if (i > 0) ss << " x";
            if (i > 1) ss << "^" << i;
        }
        ss << " $\n\n";
    }

    if (reportData.uncertaintyCalculated) {
        ss << "== 不确定度分析\n\n";
        ss << "- A类不确定度 ($u_A$): " << reportData.uncertaintyA << "\n";
        ss << "- B类不确定度 ($u_B$): " << reportData.uncertaintyB << "\n";
        ss << "- *总不确定度* ($u$): " << reportData.totalUncertainty << "\n\n";
    }

    return ss.str();
}