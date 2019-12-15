#pragma once
#include <cstdint>

using u8 =  std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using s8 =  std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using u8_fast  = std::uint_fast8_t;
using u16_fast = std::uint_fast16_t;
using u32_fast = std::uint_fast32_t;
using u64_fast = std::uint_fast64_t;

using s8_fast  = std::int_fast8_t;
using s16_fast = std::int_fast16_t;
using s32_fast = std::int_fast32_t;
using s64_fast = std::int_fast64_t;

template <typename OutputIteratorType, typename InputIteratorType>
void interleave(
        InputIteratorType left, 
        InputIteratorType right, 
        OutputIteratorType output,
        size_t length) {
    for (; length > 0; --length) {
        *(output++) = *(left++);
        *(output++) = *(right++);
    }
}
