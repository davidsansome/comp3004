#include "utilities.h"

#include <math.h>
#include <iostream>

void Utilities::fatal(const string& message)
{
	cerr << message << endl;
	exit(1);
}

void Utilities::normalizeVector(float* v)
{
	float len;
	
	len = sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]));

	v[0] = v[0] / len;
	v[1] = v[1] / len;
	v[2] = v[2] / len;
}

float Utilities::interpolate(float tl, float tr, float bl, float br,
                             float weightX, float weightY, float cWeightX, float cWeightY)
{
	return weightX * weightY * br +
	       cWeightX * weightY * bl +
	       weightX * cWeightY * tr +
	       cWeightX * cWeightY * tl;
}

float Utilities::clamp(float v, float lower, float upper)
{
	return min(upper, max(lower, v));
}

string Utilities::readString(istream& stream)
{
	uint length = read<uint8>(stream);
	char* cstr = new char[length+1];
	
	for (uint i=0 ; i<length ; ++i)
		cstr[i] = read<char>(stream);
	
	cstr[length] = '\0';
	
	string ret(cstr);
	delete[] cstr;
	return ret;
}

float Utilities::sign(float v)
{
	if (v > 0.0) return 1.0;
	else return -1.0;
}
