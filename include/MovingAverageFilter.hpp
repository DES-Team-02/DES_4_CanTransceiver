#ifndef MOVING_AVERAGE_FILTER_HPP
#define MOVING_AVERAGE_FILTER_HPP

#include <vector>
#include <queue>
#include <cstdlib>


class MovingAverageFilter{
public:
    MovingAverageFilter(size_t size, size_t discardSize);
    double filter(double new_value);

private:
    size_t _size;
    size_t _discardSize;
    std::queue<double> _values;
    double _sum;
};

#endif   //MOVING_AVERAGE_FILTER_HPP
