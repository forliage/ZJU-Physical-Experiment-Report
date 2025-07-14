#pragma once 

#include <vector>
#include <iostream>

struct DataPoint {
    double x;
    double y;
};

class DataSet {
public:
    DataSet() = default;

    void addPoint(double x, double y);

    std::vector<double> getYValues() const;

    const std::vector<DataPoint>& getPoints() const;

    size_t size() const;

    void print() const;

private:
    std::vector<DataPoint> points;
};