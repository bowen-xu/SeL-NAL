#ifndef _HELPERS_H
#define _HELPERS_H

#include <vector>
#include <algorithm>
#include <cmath>


namespace HELPERS
{
    inline std::vector<double> softmax(const std::vector<double> &dist)
    {
        std::vector<double> result;
        result.reserve(dist.size());
        double sum = 0.0;

        // Calculate the sum of the exponentials of the input values
        for (double value : dist)
        {
            double exp_value = std::exp(value);
            sum += exp_value;
        }

        // Calculate the softmax for each input value and store it in the result vector
        for (double value : dist)
        {
            double softmax_value = std::exp(value) / sum;
            result.push_back(softmax_value);
        }

        return result;
    }
} // namespace HELPERS

#endif