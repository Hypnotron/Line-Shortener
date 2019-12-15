#include <cstddef>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iostream>
#include "byte-mini.hpp"

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
    DataType sum {0};
    for (size_t index {0}; index < left.size(); ++index) {
        sum += (left[index] * right[index]);
    }
    return sum;
}


class FastQueue {
    private:
        const size_t capacity;
        std::vector<size_t> queue; 
        std::vector<s16> priorityCoefficients;
        std::vector<size_t> prioritizedPartySizes;
        std::vector<size_t> sortedPartySizes;
        //TODO: Priority global subtraction (vector op)

        std::vector<size_t> permuteOptimally(
                const std::vector<size_t>& sortedSet, 
                std::function<std::vector<size_t>(
                    FastQueue*, 
                    const std::vector<size_t>&)> validate,
                std::vector<size_t> subset = {},
                size_t start = 0) {
            subset.push_back(sortedSet[start]);
            for (
                    size_t first {start + 1}; 
                    first <= sortedSet.size(); 
                    subset.back() = sortedSet[first], ++first) {
                auto tmp {validate(this, subset)}; 
                if (tmp.size() != 0) {
                    return tmp;
                }
                else {
                    const auto tmp2 {permuteOptimally(
                            sortedSet, 
                            validate, 
                            subset,
                            first
                    )}; 
                    if (tmp2.size() != 0) {
                        return tmp2;
                    }
                }
            }
            return {};
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

            prioritizedPartySizes = sortedPartySizes;
        }

        std::vector<size_t> validate(
                const std::vector<size_t>& partySizes) {
            std::vector<size_t> maximums(partySizes.size());
            auto maximum {maximums.begin()};
            for (const auto& partySize: partySizes) {
                *maximum = capacity / partySize;
                ++maximum;
            }
            
            std::vector<size_t> quantities(partySizes.size(), 1);
            while (quantities.back() <= maximums.back()) {
                if (dotProduct(quantities, partySizes) == capacity) {
                    std::vector<size_t> output (partySizes.size() * 2);
                    interleave(
                            quantities.cbegin(), 
                            partySizes.begin(), 
                            output.begin(), 
                            quantities.size());
                    return output;
                }

                ++quantities[0];
                for (size_t index {0}; index + 1 < partySizes.size(); ++index) {
                    if (quantities[index] > maximum[index]) {
                        quantities[index] = 0;
                        ++quantities[index + 1];
                    }
                    else {
                        break;
                    }
                }
            }

            return {};
        }

        void add(const size_t partySize) {
            ++queue[partySize]; 
        } 

        void reevaluatePriority(const size_t partySize) {
            size_t start {std::find(prioritizedPartySizes.begin(), prioritizedPartySizes.end(), partySize) - prioritizedPartySizes.begin()}; 
            size_t destination {start + 1};
            for (
                    ;
                    (priorityCoefficients[partySize - 1] > priorityCoefficients[prioritizedPartySizes[destination] - 1]) 
                 && (destination < capacity); 
                    ++destination) {
                prioritizedPartySizes[destination - 1] = prioritizedPartySizes[destination];
            }
            prioritizedPartySizes[destination - 1] = partySize;
        }

        //TODO: 
        std::vector<size_t> advance() {
            auto permutation {permuteOptimally(prioritizedPartySizes, &FastQueue::validate)};
            std::cout << "Pull from line: ";
            for (size_t i {0}; i < permutation.size(); i += 2) {
                std::cout << permutation[i] << "x" << permutation[i + 1] << ", ";
                priorityCoefficients[permutation[i + 1] - 1] += permutation[i] * permutation[i + 1];
                reevaluatePriority(permutation[i + 1]);
            }
            std::cout << "\n";

            std::cout << "New record:";
            for (const auto& coef: prioritizedPartySizes) {
                std::cout << " " << coef;
            }
            std::cout << "\n";

            return {};
        }
};

//TODO: Finish
int main() {
    FastQueue demo(4);
    for (size_t i {0}; i < 50; ++i) {
        demo.add(1);
        demo.add(2);
        demo.add(3);
        demo.add(4);
    }
    for (size_t i {0}; i < 50; ++i) {
        demo.advance();
    }

    return 0;
}
