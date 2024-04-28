#ifndef FDCLI_TYPES_ALIASES
#define FDCLI_TYPES_ALIASES
#pragma once

#include <cinttypes>
#include <cmath>
#include <cstddef>
#include <cstdint>


using b32 = std::uint_fast32_t;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using byte  = u8;
using word  = u16;
using dword = u32;
using qword = u64;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using signed_byte  = i8;
using signed_word  = i16;
using signed_dword = i32;
using signed_qword = i64;

using uptr = std::uintptr_t;
using iptr = std::intptr_t;

using isize = std::ptrdiff_t;
using usize = std::size_t;

using f32  = std::float_t;
using f64  = std::double_t;
using f128 = long double;

#endif /* FDCLI_TYPES_ALIASES */
