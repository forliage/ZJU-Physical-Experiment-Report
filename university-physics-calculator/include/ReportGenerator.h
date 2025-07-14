#pragma once

#include "DataSet.h"
#include "Calculator.h"
#include <string>

struct ReportData {
    DataSet data;
    Calculator::StatisticsResult stats;
    Calculator::LinearFitResult linearFit;
    Eigen::VectorXd polyCoeffs;
    int polyOrder = 0;
    double uncertaintyA = 0.0;
    double uncertaintyB = 0.0;
    double totalUncertainty = 0.0;

    bool statsCalculated = false;
    bool linearFitCalculated = false;
    bool polyFitCalculated = false;
    bool uncertaintyCalculated = false;
};


class ReportGenerator {
public:
    static std::string generateMarkdown(const ReportData& reportData);

    static std::string generateTypst(const ReportData& reportData);
};