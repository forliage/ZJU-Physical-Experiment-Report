#include "DataSet.h" 
#include <iostream>
#include <iomanip> 

void DataSet::addPoint(double x, double y) {
    points.push_back({x, y});
}

std::vector<double> DataSet::getYValues() const {
    std::vector<double> y_values;
    y_values.reserve(points.size());
    for (const auto& p : points) {
        y_values.push_back(p.y);
    }
    return y_values;
}

size_t DataSet::size() const {
    return points.size();
}

void DataSet::print() const {
    std::cout << "--------------------" << std::endl;
    std::cout << "Data Points (N=" << size() << "):" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << std::left << std::setw(10) << "X" << std::setw(10) << "Y" << std::endl;
    for (const auto& p : points) {
        std::cout << std::left << std::setw(10) << p.x << std::setw(10) << p.y << std::endl;
    }
    std::cout << "--------------------" << std::endl;
}

const std::vector<DataPoint>& DataSet::getPoints() const {
    return points;
}