template <typename DataType>
inline DataType max(DataType left, DataType right) {
    return (left > right) ? left : right;
} 

template <typename DataType>
inline DataType min(DataType left, DataType right) {
    return (left < right) ? left : right; 
}

