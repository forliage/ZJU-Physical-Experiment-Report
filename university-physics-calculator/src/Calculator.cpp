#include "Calculator.h"
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <iostream>

Calculator::StatisticsResult Calculator::calculateStatistics(const std::vector<double>& data) {
    StatisticsResult result;
    result.count = data.size();

    if (result.count == 0) {
        return result; 
    }

    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    result.mean = sum / result.count;

    if (result.count < 2) {
        return result;
    }

    double sq_sum = 0.0;
    for (const double& val : data) {
        sq_sum += std::pow(val - result.mean, 2);
    }
    result.variance = sq_sum / (result.count - 1);

    result.std_dev = std::sqrt(result.variance);

    if (std::abs(result.mean) > 1e-9) { 
        result.cv = result.std_dev / result.mean;
    }

    return result;
}

double Calculator::calculateUncertaintyA(const DataSet& dataSet) {
    if (dataSet.size() == 0) {
        return 0.0;
    }
    auto y_values = dataSet.getYValues();
    StatisticsResult stats = calculateStatistics(y_values);
    return stats.std_dev / std::sqrt(stats.count);
}

double Calculator::calculateAreaUnderCurve(const Eigen::VectorXd& coeffs, double x_start, double x_end) {
    auto indefinite_integral = [&](double x) {
        double integral_value = 0.0;
        for (int i = 0; i < coeffs.size(); ++i) {
            integral_value += (coeffs[i] / (i + 1.0)) * std::pow(x, i + 1.0);
        }
        return integral_value;
    };

    return indefinite_integral(x_end) - indefinite_integral(x_start);
}

Calculator::LinearFitResult Calculator::linearFit(const DataSet& dataSet) {
    size_t n = dataSet.size();
    if (n < 2) {
        return {};
    }

    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0, sum_y2 = 0.0;

    for (const auto& p : dataSet.getPoints()) {
        sum_x += p.x;
        sum_y += p.y;
        sum_xy += p.x * p.y;
        sum_x2 += p.x * p.x;
        sum_y2 += p.y * p.y;
    }

    LinearFitResult result; 
    double denominator = n * sum_x2 - sum_x * sum_x;

    if (std::abs(denominator) < 1e-9) {
        return {};
    }

    result.slope_a = (n * sum_xy - sum_x * sum_y) / denominator;
    result.intercept_b = (sum_y - result.slope_a * sum_x) / n;
    
    double r_denominator = (n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y);
    if (std::abs(r_denominator) < 1e-9) {
        result.r_squared = 1.0;
    } else {
        double r_numerator = n * sum_xy - sum_x * sum_y;
        result.r_squared = (r_numerator * r_numerator) / r_denominator;
    }
    
    return result;
}

Eigen::VectorXd Calculator::polynomialFit(const DataSet& dataSet, int order) {
    if (order < 1) {
        throw std::invalid_argument("Polynomial order must be at least 1.");
    }

    const auto& points = dataSet.getPoints();
    size_t num_points = points.size();

    if (num_points < static_cast<size_t>(order + 1)) {
        throw std::runtime_error("Number of data points must be at least (order + 1).");
    }

    int matrix_size = order + 1;

    Eigen::MatrixXd A(matrix_size, matrix_size);
    A.setZero();

    Eigen::VectorXd B(matrix_size);
    B.setZero();

    std::vector<double> sum_x_pow(2 * order + 1, 0.0);
    for (const auto& p : points) {
        for (int k = 0; k <= 2 * order; ++k) {
            sum_x_pow[k] += std::pow(p.x, k);
        }
    }

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            A(i, j) = sum_x_pow[i + j];
        }
    }

    for (int i = 0; i < matrix_size; ++i) {
        double sum_yx_pow = 0.0;
        for (const auto& p : points) {
            sum_yx_pow += p.y * std::pow(p.x, i);
        }
        B(i) = sum_yx_pow;
    }

    Eigen::VectorXd coefficients = A.colPivHouseholderQr().solve(B);

    return coefficients;
}

double Calculator::combineUncertainties(const std::vector<double>& uncertainties) {
    double sum_of_squares = 0.0;
    for (const double u : uncertainties) {
        sum_of_squares += u * u;
    }
    return std::sqrt(sum_of_squares);
}

double Calculator::calculateTotalUncertainty(double uncertainty_a, double uncertainty_b) {
    return std::sqrt(uncertainty_a * uncertainty_a + uncertainty_b * uncertainty_b);
}