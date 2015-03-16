#ifndef UTILITIES_H
#define UTILITIES_H

#include "types.h"

namespace Utilities
{
	float3 normalize(float3 v);
	
	float dot(float3 a, float3 b);
	float clamp(float min, float value, float max);
	float3 multMatrixInvTrans(float3 vector);
	float3 multMatrixTrans(float3 vector);
	
	void invert2(float* mat, float* dst);
	void transpose(float* m);
};

#endif
