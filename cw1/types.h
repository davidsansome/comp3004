#ifndef TYPES_H
#define TYPES_H

#include <math.h>
#include <stdarg.h>

#include "array.h"

typedef array<float, 4> float4;
typedef array<float, 3> float3;
typedef array<float, 2> float2;

const float TWO_PI = M_PI * 2.0;

template<int N>
array<float, N> floatn(double f, ...)
{
	va_list ap;
	va_start(ap, f);
	array<float, N> ret;
	ret[0] = f;
	for (int i=1; i<N ; ++i)
		ret[i] = va_arg(ap, double);
	va_end(ap);
	return ret;
}

#endif
