#pragma once

#include "unique_function.h"
#include "gsl/gsl"


using gsl::not_null;
using index_type = gsl::index;
using size_type  = gsl::index;

template <typename Signature> using Fn = std::function<Signature>;

template <typename Signature> using FnMut = base::unique_function<Signature>;

//using uchar = unsigned char; // Qt has uchar
using int8    = int8_t;
using uint8   = uint8_t;
using int16   = int16_t;
using uint16  = uint16_t;
using int32   = int32_t;
using uint32  = uint32_t;
using int64   = int64_t;
using uint64  = uint64_t;
using float32 = float;
using float64 = double;

using TimeId = int32;

#ifndef _DEBUG

#    ifdef _MSC_VER
#        define DESKTOP_APP_USE_NO_ALLOCATION_LITERAL
#    endif // _MSC_VER

#endif // !_DEBUG
