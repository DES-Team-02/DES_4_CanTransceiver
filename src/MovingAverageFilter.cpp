#include "MovingAverageFilter.hpp"


MovingAverageFilter::MovingAverageFilter(size_t size, size_t discardSize)
    : _size(size), _discardSize(discardSize), _sum(0) {}


double MovingAverageFilter::filter(double new_value){
    if(_values.size() == _size){
        for(size_t i = 0 ; i < _discardSize; ++i){
            _sum -= _values.front();
            _values.pop();
        }
    }

    _values.push(new_value);
    _sum += new_value;

    return _sum / _values.size();
}
