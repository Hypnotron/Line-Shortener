#include <cstddef>
#include <vector>
#include <functional>
#include <numeric>
#include <iostream>
//TODO: Remove iostream
#include "fixed-width.hpp"

template <typename DataType>
std::vector<DataType> sub(const std::vector<DataType>& vec, size_t start, size_t end) {
    std::vector<DataType> result;
    result.reserve(end - start);
    for (size_t i {start}; i != end; ++i) {
        result.push_back(vec[i]);
    }
    return result;
}

template <typename DataType>
std::vector<DataType> concat(const std::vector<DataType>& left, const std::vector<DataType>& right) {
    std::vector<DataType> result(left);
    left.insert(left.end(), right.begin(), right.end());
    return result;
}

template <typename DataType>
DataType dotProduct(const std::vector<DataType>& left, const std::vector<DataType>& right) {
    DataType sum;
    for (size_t index {0}; index < left.size(); ++index) {
        sum += left[index] * right[index];
    }
    return sum;
}


class FastQueue {
    private:
        const size_t capacity;
        std::vector<size_t> queue; 
        std::vector<s16> priorityCoefficients;
        std::vector<size_t> sortedPartySizes;
        //TODO: Priority global subtraction (vector op)
        //TODO: Validation function

        std::vector<size_t> permuteOptimally(
                const std::vector<size_t>& sortedSet, 
                std::function<bool(FastQueue*, const std::vector<size_t>&)> isValid,
                std::vector<size_t> subset = {},
                size_t start = 0) {
            subset.push_back(0);
            for (size_t first {start}; first < sortedSet.size(); ++first) {
                subset.back() = sortedSet[first];
                if (isValid(this, subset)) { 
                    return subset; 
                }
                else if (first + 1 < sortedSet.size()) {
                    auto tmp {permuteOptimally(sortedSet, isValid, subset, first + 1)}; 
                    //Only return if non-empty (explained below):
                    if (tmp.size() != 0) {
                        return tmp;
                    }
                }
                else {
                    //Empty set: acts as a sentinel that indicates 
                    //that the algorithm has recursed maximally.
                    return {};
                }

            }
        }

    public:
        FastQueue(const size_t capacity)
              : capacity{capacity} {
            queue.resize(capacity);
            std::fill(queue.begin(), queue.end(), 0);

            priorityCoefficients.resize(capacity);
            std::fill(priorityCoefficients.begin(), priorityCoefficients.end(), 0);

            sortedPartySizes.resize(capacity);
            std::iota(sortedPartySizes.begin(), sortedPartySizes.end(), 1);
            
            permuteOptimally({1, 2, 3, 4, 5}, &FastQueue::validate);
        }

        //TODO: Fix below
        bool validate(const std::vector<size_t>& partySizes) {
            std::vector<size_t> maximums(partySizes.size());
            auto maximum {maximums.begin()};
            for (const auto& partySize: partySizes) {
                *maximum = capacity / partySize;
                ++maximum;
            }
            
            std::vector<size_t> quantities(partySizes.size(), 0);
            while (quantities.back() <= maximums.back()) {
                if (dotProduct(quantities, partySizes) == capacity) {
                    return true;
                }

                ++quantities[0];
                for (size_t index {0}; index < partySizes.size() - 1; ++index) {
                    if (quantities[index] > maximum[index]) {
                        quantities[index] = 0;
                        ++quantities[index + 1];
                    }
                    else {
                        break;
                    }
                }
            }

            return false;
        }

        void add(const size_t partySize) {
            ++queue[partySize]; 
        } 

        void reevaluatePriority(const size_t partySize) {
            size_t destination {partySize + 1}; 
            for (
                    ;
                    priorityCoefficients[partySize] > priorityCoefficients[destination] 
                 && destination < capacity; 
                    ++destination) {
                sortedPartySizes[destination - 1] = sortedPartySizes[destination];
            }
            sortedPartySizes[destination - 1] = partySize;
        }

        //TODO: 
        std::vector<size_t> advance() {
            return {};
        }
};

//TODO: Fix
int main() {
    FastQueue demo(5);

    return 0;
}
